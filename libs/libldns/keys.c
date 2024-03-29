/*
 * keys.c handle private keys for use in DNSSEC
 *
 * This module should hide some of the openSSL complexities
 * and give a general interface for private keys and hmac
 * handling
 *
 * (c) NLnet Labs, 2004-2006
 *
 * See the file LICENSE for the license
 */

#include <ldns/config.h>

#include <ldns/ldns.h>

#ifdef HAVE_SSL
#include <openssl/ssl.h>
#include <openssl/engine.h>
#include <openssl/rand.h>
#endif /* HAVE_SSL */

#ifdef ANDROID
RSA *RSA_generate_key(int bits, unsigned long e_value,
        void (*callback)(int,int,void *), void *cb_arg)
{
    BN_GENCB cb;
    int i;
    RSA *rsa = RSA_new();
    BIGNUM *e = BN_new();

    if(!rsa || !e) goto err;

    /* The problem is when building with 8, 16, or 32 BN_ULONG,
     * unsigned long can be larger */
    for (i=0; i<(int)sizeof(unsigned long)*8; i++)
    {
        if (e_value & (1UL<<i))
            if (BN_set_bit(e,i) == 0)
                goto err;
    }

    BN_GENCB_set_old(&cb, callback, cb_arg);

    if(RSA_generate_key_ex(rsa, bits, e, &cb)) {
        BN_free(e);
        return rsa;
    }
err:
    if(e) BN_free(e);
    if(rsa) RSA_free(rsa);
    return 0;
}
#endif

ldns_lookup_table ldns_signing_algorithms[] = {
        { LDNS_SIGN_RSAMD5, "RSAMD5" },
        { LDNS_SIGN_RSASHA1, "RSASHA1" },
        { LDNS_SIGN_RSASHA1_NSEC3, "RSASHA1-NSEC3-SHA1" },
#ifdef USE_SHA2
        { LDNS_SIGN_RSASHA256, "RSASHA256" },
        { LDNS_SIGN_RSASHA512, "RSASHA512" },
#endif
#ifdef USE_GOST
        { LDNS_SIGN_ECC_GOST, "ECC-GOST" },
#endif
#ifdef USE_ECDSA
        { LDNS_SIGN_ECDSAP256SHA256, "ECDSAP256SHA256" },
        { LDNS_SIGN_ECDSAP384SHA384, "ECDSAP384SHA384" },
#endif
        { LDNS_SIGN_DSA, "DSA" },
        { LDNS_SIGN_DSA_NSEC3, "DSA-NSEC3-SHA1" },
        { LDNS_SIGN_HMACMD5, "hmac-md5.sig-alg.reg.int" },
        { LDNS_SIGN_HMACSHA1, "hmac-sha1" },
        { LDNS_SIGN_HMACSHA256, "hmac-sha256" },
        { 0, NULL }
};

ldns_key_list *
ldns_key_list_new(void)
{
	ldns_key_list *key_list = LDNS_MALLOC(ldns_key_list);
	if (!key_list) {
		return NULL;
	} else {
		key_list->_key_count = 0;
		key_list->_keys = NULL;
		return key_list;
	}
}

ldns_key *
ldns_key_new(void)
{
	ldns_key *newkey;

	newkey = LDNS_MALLOC(ldns_key);
	if (!newkey) {
		return NULL;
	} else {
		/* some defaults - not sure wether to do this */
		ldns_key_set_use(newkey, true);
		ldns_key_set_flags(newkey, LDNS_KEY_ZONE_KEY);
		ldns_key_set_origttl(newkey, 0);
		ldns_key_set_keytag(newkey, 0);
		ldns_key_set_inception(newkey, 0);
		ldns_key_set_expiration(newkey, 0);
		ldns_key_set_pubkey_owner(newkey, NULL);
#ifdef HAVE_SSL
		ldns_key_set_evp_key(newkey, NULL);
#endif /* HAVE_SSL */
		ldns_key_set_hmac_key(newkey, NULL);
		ldns_key_set_external_key(newkey, NULL);
		return newkey;
	}
}

ldns_status
ldns_key_new_frm_fp(ldns_key **k, FILE *fp)
{
	return ldns_key_new_frm_fp_l(k, fp, NULL);
}

#ifdef HAVE_SSL
ldns_status
ldns_key_new_frm_engine(ldns_key **key, ENGINE *e, char *key_id, ldns_algorithm alg)
{
	ldns_key *k;

	k = ldns_key_new();
        if(!k) return LDNS_STATUS_MEM_ERR;
#ifndef S_SPLINT_S
	k->_key.key = ENGINE_load_private_key(e, key_id, UI_OpenSSL(), NULL);
        if(!k->_key.key) {
                ldns_key_free(k);
                return LDNS_STATUS_ERR;
        }
	ldns_key_set_algorithm(k, (ldns_signing_algorithm) alg);
	if (!k->_key.key) {
                ldns_key_free(k);
		return LDNS_STATUS_ENGINE_KEY_NOT_LOADED;
	} 
#endif /* splint */
	*key = k;
	return LDNS_STATUS_OK;
}
#endif

#ifdef USE_GOST
/** store GOST engine reference loaded into OpenSSL library */
ENGINE* ldns_gost_engine = NULL;

int
ldns_key_EVP_load_gost_id(void)
{
	static int gost_id = 0;
	const EVP_PKEY_ASN1_METHOD* meth;
	ENGINE* e;

	if(gost_id) return gost_id;

	/* see if configuration loaded gost implementation from other engine*/
	meth = EVP_PKEY_asn1_find_str(NULL, "gost2001", -1);
	if(meth) {
		EVP_PKEY_asn1_get0_info(&gost_id, NULL, NULL, NULL, NULL, meth);
		return gost_id;
	}

	/* see if engine can be loaded already */
	e = ENGINE_by_id("gost");
	if(!e) {
		/* load it ourself, in case statically linked */
		ENGINE_load_builtin_engines();
		ENGINE_load_dynamic();
		e = ENGINE_by_id("gost");
	}
	if(!e) {
		/* no gost engine in openssl */
		return 0;
	}
	if(!ENGINE_set_default(e, ENGINE_METHOD_ALL)) {
		ENGINE_finish(e);
		ENGINE_free(e);
		return 0;
	}

	meth = EVP_PKEY_asn1_find_str(&e, "gost2001", -1);
	if(!meth) {
		/* algo not found */
		ENGINE_finish(e);
		ENGINE_free(e);
		return 0;
	}
        /* Note: do not ENGINE_finish and ENGINE_free the acquired engine
         * on some platforms this frees up the meth and unloads gost stuff */
        ldns_gost_engine = e;
	
	EVP_PKEY_asn1_get0_info(&gost_id, NULL, NULL, NULL, NULL, meth);
	return gost_id;
} 

void ldns_key_EVP_unload_gost(void)
{
        if(ldns_gost_engine) {
                ENGINE_finish(ldns_gost_engine);
                ENGINE_free(ldns_gost_engine);
                ldns_gost_engine = NULL;
        }
}

/** read GOST private key */
static EVP_PKEY*
ldns_key_new_frm_fp_gost_l(FILE* fp, int* line_nr)
{
	char token[16384];
	const unsigned char* pp;
	int gost_id;
	EVP_PKEY* pkey;
	ldns_rdf* b64rdf = NULL;

	gost_id = ldns_key_EVP_load_gost_id();
	if(!gost_id)
		return NULL;

	if (ldns_fget_keyword_data_l(fp, "GostAsn1", ": ", token, "\n", 
		sizeof(token), line_nr) == -1)
		return NULL;
	while(strlen(token) < 96) {
		/* read more b64 from the file, b64 split on multiple lines */
		if(ldns_fget_token_l(fp, token+strlen(token), "\n",
			sizeof(token)-strlen(token), line_nr) == -1)
			return NULL;
	}
	if(ldns_str2rdf_b64(&b64rdf, token) != LDNS_STATUS_OK)
		return NULL;
	pp = (unsigned char*)ldns_rdf_data(b64rdf);
	pkey = d2i_PrivateKey(gost_id, NULL, &pp, (int)ldns_rdf_size(b64rdf));
	ldns_rdf_deep_free(b64rdf);
	return pkey;
}
#endif

#ifdef USE_ECDSA
/** calculate public key from private key */
static int
ldns_EC_KEY_calc_public(EC_KEY* ec)
{
        EC_POINT* pub_key;
        const EC_GROUP* group;
        group = EC_KEY_get0_group(ec);
        pub_key = EC_POINT_new(group);
        if(!pub_key) return 0;
        if(!EC_POINT_copy(pub_key, EC_GROUP_get0_generator(group))) {
                EC_POINT_free(pub_key);
                return 0;
        }
        if(!EC_POINT_mul(group, pub_key, EC_KEY_get0_private_key(ec),
                NULL, NULL, NULL)) {
                EC_POINT_free(pub_key);
                return 0;
        }
        if(EC_KEY_set_public_key(ec, pub_key) == 0) {
                EC_POINT_free(pub_key);
                return 0;
        }
        EC_POINT_free(pub_key);
        return 1;
}

/** read ECDSA private key */
static EVP_PKEY*
ldns_key_new_frm_fp_ecdsa_l(FILE* fp, ldns_algorithm alg, int* line_nr)
{
	char token[16384];
        ldns_rdf* b64rdf = NULL;
        unsigned char* pp;
        BIGNUM* bn;
        EVP_PKEY* evp_key;
        EC_KEY* ec;
	if (ldns_fget_keyword_data_l(fp, "PrivateKey", ": ", token, "\n", 
		sizeof(token), line_nr) == -1)
		return NULL;
	if(ldns_str2rdf_b64(&b64rdf, token) != LDNS_STATUS_OK)
		return NULL;
        pp = (unsigned char*)ldns_rdf_data(b64rdf);

        if(alg == LDNS_ECDSAP256SHA256)
                ec = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
        else if(alg == LDNS_ECDSAP384SHA384)
                ec = EC_KEY_new_by_curve_name(NID_secp384r1);
        else    ec = NULL;
        if(!ec) {
	        ldns_rdf_deep_free(b64rdf);
                return NULL;
        }
	bn = BN_bin2bn(pp, (int)ldns_rdf_size(b64rdf), NULL);
	ldns_rdf_deep_free(b64rdf);
        if(!bn) {
                EC_KEY_free(ec);
                return NULL;
        }
        EC_KEY_set_private_key(ec, bn);
        BN_free(bn);
        if(!ldns_EC_KEY_calc_public(ec)) {
                EC_KEY_free(ec);
                return NULL;
        }

        evp_key = EVP_PKEY_new();
        if(!evp_key) {
                EC_KEY_free(ec);
                return NULL;
        }
        if (!EVP_PKEY_assign_EC_KEY(evp_key, ec)) {
		EVP_PKEY_free(evp_key);
                EC_KEY_free(ec);
                return NULL;
	}
        return evp_key;
}
#endif
	
ldns_status
ldns_key_new_frm_fp_l(ldns_key **key, FILE *fp, int *line_nr)
{
	ldns_key *k;
	char *d;
	ldns_signing_algorithm alg;
	ldns_rr *key_rr;
#ifdef HAVE_SSL
	RSA *rsa;
	DSA *dsa;
	unsigned char *hmac;
	size_t hmac_size;
#endif /* HAVE_SSL */

	k = ldns_key_new();

	d = LDNS_XMALLOC(char, LDNS_MAX_LINELEN);
	if (!k || !d) {
                ldns_key_free(k);
                LDNS_FREE(d);
		return LDNS_STATUS_MEM_ERR;
	}

	alg = 0;

	/* the file is highly structured. Do this in sequence */
	/* RSA:
	 * Private-key-format: v1.x.
 	 * Algorithm: 1 (RSA)

	 */
	/* get the key format version number */
	if (ldns_fget_keyword_data_l(fp, "Private-key-format", ": ", d, "\n",
				LDNS_MAX_LINELEN, line_nr) == -1) {
		/* no version information */
                ldns_key_free(k);
                LDNS_FREE(d);
		return LDNS_STATUS_SYNTAX_ERR;
	}
	if (strncmp(d, "v1.", 3) != 0) {
                ldns_key_free(k);
                LDNS_FREE(d);
		return LDNS_STATUS_SYNTAX_VERSION_ERR;
	}

	/* get the algorithm type, our file function strip ( ) so there are
	 * not in the return string! */
	if (ldns_fget_keyword_data_l(fp, "Algorithm", ": ", d, "\n",
				LDNS_MAX_LINELEN, line_nr) == -1) {
		/* no alg information */
                ldns_key_free(k);
                LDNS_FREE(d);
		return LDNS_STATUS_SYNTAX_ALG_ERR;
	}

	if (strncmp(d, "1 RSA", 2) == 0) {
		alg = LDNS_SIGN_RSAMD5;
	}
	if (strncmp(d, "2 DH", 2) == 0) {
		alg = (ldns_signing_algorithm)LDNS_DH;
	}
	if (strncmp(d, "3 DSA", 2) == 0) {
		alg = LDNS_SIGN_DSA;
	}
	if (strncmp(d, "4 ECC", 2) == 0) {
		alg = (ldns_signing_algorithm)LDNS_ECC;
	}
	if (strncmp(d, "5 RSASHA1", 2) == 0) {
		alg = LDNS_SIGN_RSASHA1;
	}
	if (strncmp(d, "6 DSA", 2) == 0) {
		alg = LDNS_SIGN_DSA_NSEC3;
	}
	if (strncmp(d, "7 RSASHA1", 2) == 0) {
		alg = LDNS_SIGN_RSASHA1_NSEC3;
	}

	if (strncmp(d, "8 RSASHA256", 2) == 0) {
#ifdef USE_SHA2
		alg = LDNS_SIGN_RSASHA256;
#else
# ifdef STDERR_MSGS
		fprintf(stderr, "Warning: SHA256 not compiled into this ");
		fprintf(stderr, "version of ldns\n");
# endif
#endif
	}
	if (strncmp(d, "10 RSASHA512", 3) == 0) {
#ifdef USE_SHA2
		alg = LDNS_SIGN_RSASHA512;
#else
# ifdef STDERR_MSGS
		fprintf(stderr, "Warning: SHA512 not compiled into this ");
		fprintf(stderr, "version of ldns\n");
# endif
#endif
	}
	if (strncmp(d, "12 ECC-GOST", 3) == 0) {
#ifdef USE_GOST
		alg = LDNS_SIGN_ECC_GOST;
#else
# ifdef STDERR_MSGS
		fprintf(stderr, "Warning: ECC-GOST not compiled into this ");
		fprintf(stderr, "version of ldns, use --enable-gost\n");
# endif
#endif
	}
	if (strncmp(d, "13 ECDSAP256SHA256", 3) == 0) {
#ifdef USE_ECDSA
                alg = LDNS_SIGN_ECDSAP256SHA256;
#else
# ifdef STDERR_MSGS
		fprintf(stderr, "Warning: ECDSA not compiled into this ");
		fprintf(stderr, "version of ldns, use --enable-ecdsa\n");
# endif
#endif
        }
	if (strncmp(d, "14 ECDSAP384SHA384", 3) == 0) {
#ifdef USE_ECDSA
                alg = LDNS_SIGN_ECDSAP384SHA384;
#else
# ifdef STDERR_MSGS
		fprintf(stderr, "Warning: ECDSA not compiled into this ");
		fprintf(stderr, "version of ldns, use --enable-ecdsa\n");
# endif
#endif
        }
	if (strncmp(d, "157 HMAC-MD5", 4) == 0) {
		alg = LDNS_SIGN_HMACMD5;
	}
	if (strncmp(d, "158 HMAC-SHA1", 4) == 0) {
		alg = LDNS_SIGN_HMACSHA1;
	}
	if (strncmp(d, "159 HMAC-SHA256", 4) == 0) {
		alg = LDNS_SIGN_HMACSHA256;
	}

	LDNS_FREE(d);

	switch(alg) {
		case LDNS_SIGN_RSAMD5:
		case LDNS_SIGN_RSASHA1:
		case LDNS_SIGN_RSASHA1_NSEC3:
#ifdef USE_SHA2
		case LDNS_SIGN_RSASHA256:
		case LDNS_SIGN_RSASHA512:
#endif
			ldns_key_set_algorithm(k, alg);
#ifdef HAVE_SSL
			rsa = ldns_key_new_frm_fp_rsa_l(fp, line_nr);
			if (!rsa) {
				ldns_key_free(k);
				return LDNS_STATUS_ERR;
			}
			ldns_key_assign_rsa_key(k, rsa);
#endif /* HAVE_SSL */
			break;
		case LDNS_SIGN_DSA:
		case LDNS_SIGN_DSA_NSEC3:
			ldns_key_set_algorithm(k, alg);
#ifdef HAVE_SSL
			dsa = ldns_key_new_frm_fp_dsa_l(fp, line_nr);
			if (!dsa) {
				ldns_key_free(k);
				return LDNS_STATUS_ERR;
			}
			ldns_key_assign_dsa_key(k, dsa);
#endif /* HAVE_SSL */
			break;
		case LDNS_SIGN_HMACMD5:
		case LDNS_SIGN_HMACSHA1:
		case LDNS_SIGN_HMACSHA256:
			ldns_key_set_algorithm(k, alg);
#ifdef HAVE_SSL
			hmac = ldns_key_new_frm_fp_hmac_l(fp, line_nr, &hmac_size);
			if (!hmac) {
				ldns_key_free(k);
				return LDNS_STATUS_ERR;
			}
			ldns_key_set_hmac_size(k, hmac_size);
			ldns_key_set_hmac_key(k, hmac);
#endif /* HAVE_SSL */
			break;
		case LDNS_SIGN_ECC_GOST:
			ldns_key_set_algorithm(k, alg);
#if defined(HAVE_SSL) && defined(USE_GOST)
                        if(!ldns_key_EVP_load_gost_id()) {
				ldns_key_free(k);
                                return LDNS_STATUS_CRYPTO_ALGO_NOT_IMPL;
                        }
			ldns_key_set_evp_key(k, 
				ldns_key_new_frm_fp_gost_l(fp, line_nr));
#ifndef S_SPLINT_S
			if(!k->_key.key) {
				ldns_key_free(k);
				return LDNS_STATUS_ERR;
			}
#endif /* splint */
#endif
			break;
#ifdef USE_ECDSA
               case LDNS_SIGN_ECDSAP256SHA256:
               case LDNS_SIGN_ECDSAP384SHA384:
                        ldns_key_set_algorithm(k, alg);
                        ldns_key_set_evp_key(k,
                                ldns_key_new_frm_fp_ecdsa_l(fp, (ldns_algorithm)alg, line_nr));
#ifndef S_SPLINT_S
			if(!k->_key.key) {
				ldns_key_free(k);
				return LDNS_STATUS_ERR;
			}
#endif /* splint */
			break;
#endif
		default:
			ldns_key_free(k);
			return LDNS_STATUS_SYNTAX_ALG_ERR;
	}
	key_rr = ldns_key2rr(k);
	ldns_key_set_keytag(k, ldns_calc_keytag(key_rr));
	ldns_rr_free(key_rr);

	if (key) {
		*key = k;
		return LDNS_STATUS_OK;
	}
	ldns_key_free(k);
	return LDNS_STATUS_ERR;
}

#ifdef HAVE_SSL
RSA *
ldns_key_new_frm_fp_rsa(FILE *f)
{
	return ldns_key_new_frm_fp_rsa_l(f, NULL);
}

RSA *
ldns_key_new_frm_fp_rsa_l(FILE *f, int *line_nr)
{
	/* we parse
 	 * Modulus:
 	 * PublicExponent:
 	 * PrivateExponent:
 	 * Prime1:
 	 * Prime2:
 	 * Exponent1:
 	 * Exponent2:
 	 * Coefficient:
	 *
	 * man 3 RSA:
	 *
	 * struct
         *     {
         *     BIGNUM *n;              // public modulus
         *     BIGNUM *e;              // public exponent
         *     BIGNUM *d;              // private exponent
         *     BIGNUM *p;              // secret prime factor
         *     BIGNUM *q;              // secret prime factor
         *     BIGNUM *dmp1;           // d mod (p-1)
         *     BIGNUM *dmq1;           // d mod (q-1)
         *     BIGNUM *iqmp;           // q^-1 mod p
         *     // ...
	 *
	 */
	char *d;
	RSA *rsa;
	uint8_t *buf;
	int i;

	d = LDNS_XMALLOC(char, LDNS_MAX_LINELEN);
	buf = LDNS_XMALLOC(uint8_t, LDNS_MAX_LINELEN);
	rsa = RSA_new();
	if (!d || !rsa || !buf) {
                goto error;
	}

	/* I could use functions again, but that seems an overkill,
	 * allthough this also looks tedious
	 */

	/* Modules, rsa->n */
	if (ldns_fget_keyword_data_l(f, "Modulus", ": ", d, "\n", LDNS_MAX_LINELEN, line_nr) == -1) {
		goto error;
	}
	i = ldns_b64_pton((const char*)d, buf, ldns_b64_ntop_calculate_size(strlen(d)));
#ifndef S_SPLINT_S
	rsa->n = BN_bin2bn((const char unsigned*)buf, i, NULL);
	if (!rsa->n) {
		goto error;
	}

	/* PublicExponent, rsa->e */
	if (ldns_fget_keyword_data_l(f, "PublicExponent", ": ", d, "\n", LDNS_MAX_LINELEN, line_nr) == -1) {
		goto error;
	}
	i = ldns_b64_pton((const char*)d, buf, ldns_b64_ntop_calculate_size(strlen(d)));
	rsa->e = BN_bin2bn((const char unsigned*)buf, i, NULL);
	if (!rsa->e) {
		goto error;
	}

	/* PrivateExponent, rsa->d */
	if (ldns_fget_keyword_data_l(f, "PrivateExponent", ": ", d, "\n", LDNS_MAX_LINELEN, line_nr) == -1) {
		goto error;
	}
	i = ldns_b64_pton((const char*)d, buf, ldns_b64_ntop_calculate_size(strlen(d)));
	rsa->d = BN_bin2bn((const char unsigned*)buf, i, NULL);
	if (!rsa->d) {
		goto error;
	}

	/* Prime1, rsa->p */
	if (ldns_fget_keyword_data_l(f, "Prime1", ": ", d, "\n", LDNS_MAX_LINELEN, line_nr) == -1) {
		goto error;
	}
	i = ldns_b64_pton((const char*)d, buf, ldns_b64_ntop_calculate_size(strlen(d)));
	rsa->p = BN_bin2bn((const char unsigned*)buf, i, NULL);
	if (!rsa->p) {
		goto error;
	}

	/* Prime2, rsa->q */
	if (ldns_fget_keyword_data_l(f, "Prime2", ": ", d, "\n", LDNS_MAX_LINELEN, line_nr) == -1) {
		goto error;
	}
	i = ldns_b64_pton((const char*)d, buf, ldns_b64_ntop_calculate_size(strlen(d)));
	rsa->q = BN_bin2bn((const char unsigned*)buf, i, NULL);
	if (!rsa->q) {
		goto error;
	}

	/* Exponent1, rsa->dmp1 */
	if (ldns_fget_keyword_data_l(f, "Exponent1", ": ", d, "\n", LDNS_MAX_LINELEN, line_nr) == -1) {
		goto error;
	}
	i = ldns_b64_pton((const char*)d, buf, ldns_b64_ntop_calculate_size(strlen(d)));
	rsa->dmp1 = BN_bin2bn((const char unsigned*)buf, i, NULL);
	if (!rsa->dmp1) {
		goto error;
	}

	/* Exponent2, rsa->dmq1 */
	if (ldns_fget_keyword_data_l(f, "Exponent2", ": ", d, "\n", LDNS_MAX_LINELEN, line_nr) == -1) {
		goto error;
	}
	i = ldns_b64_pton((const char*)d, buf, ldns_b64_ntop_calculate_size(strlen(d)));
	rsa->dmq1 = BN_bin2bn((const char unsigned*)buf, i, NULL);
	if (!rsa->dmq1) {
		goto error;
	}

	/* Coefficient, rsa->iqmp */
	if (ldns_fget_keyword_data_l(f, "Coefficient", ": ", d, "\n", LDNS_MAX_LINELEN, line_nr) == -1) {
		goto error;
	}
	i = ldns_b64_pton((const char*)d, buf, ldns_b64_ntop_calculate_size(strlen(d)));
	rsa->iqmp = BN_bin2bn((const char unsigned*)buf, i, NULL);
	if (!rsa->iqmp) {
		goto error;
	}
#endif /* splint */

	LDNS_FREE(buf);
	LDNS_FREE(d);
	return rsa;

error:
	RSA_free(rsa);
	LDNS_FREE(d);
	LDNS_FREE(buf);
	return NULL;
}

DSA *
ldns_key_new_frm_fp_dsa(FILE *f)
{
	return ldns_key_new_frm_fp_dsa_l(f, NULL);
}

DSA *
ldns_key_new_frm_fp_dsa_l(FILE *f, ATTR_UNUSED(int *line_nr))
{
	int i;
	char *d;
	DSA *dsa;
	uint8_t *buf;

	d = LDNS_XMALLOC(char, LDNS_MAX_LINELEN);
	buf = LDNS_XMALLOC(uint8_t, LDNS_MAX_LINELEN);
	dsa = DSA_new();
	if (!d || !dsa || !buf) {
                goto error;
	}

	/* the line parser removes the () from the input... */

	/* Prime, dsa->p */
	if (ldns_fget_keyword_data_l(f, "Primep", ": ", d, "\n", LDNS_MAX_LINELEN, line_nr) == -1) {
		goto error;
	}
	i = ldns_b64_pton((const char*)d, buf, ldns_b64_ntop_calculate_size(strlen(d)));
#ifndef S_SPLINT_S
	dsa->p = BN_bin2bn((const char unsigned*)buf, i, NULL);
	if (!dsa->p) {
		goto error;
	}

	/* Subprime, dsa->q */
	if (ldns_fget_keyword_data_l(f, "Subprimeq", ": ", d, "\n", LDNS_MAX_LINELEN, line_nr) == -1) {
		goto error;
	}
	i = ldns_b64_pton((const char*)d, buf, ldns_b64_ntop_calculate_size(strlen(d)));
	dsa->q = BN_bin2bn((const char unsigned*)buf, i, NULL);
	if (!dsa->q) {
		goto error;
	}

	/* Base, dsa->g */
	if (ldns_fget_keyword_data_l(f, "Baseg", ": ", d, "\n", LDNS_MAX_LINELEN, line_nr) == -1) {
		goto error;
	}
	i = ldns_b64_pton((const char*)d, buf, ldns_b64_ntop_calculate_size(strlen(d)));
	dsa->g = BN_bin2bn((const char unsigned*)buf, i, NULL);
	if (!dsa->g) {
		goto error;
	}

	/* Private key, dsa->priv_key */
	if (ldns_fget_keyword_data_l(f, "Private_valuex", ": ", d, "\n", LDNS_MAX_LINELEN, line_nr) == -1) {
		goto error;
	}
	i = ldns_b64_pton((const char*)d, buf, ldns_b64_ntop_calculate_size(strlen(d)));
	dsa->priv_key = BN_bin2bn((const char unsigned*)buf, i, NULL);
	if (!dsa->priv_key) {
		goto error;
	}

	/* Public key, dsa->priv_key */
	if (ldns_fget_keyword_data_l(f, "Public_valuey", ": ", d, "\n", LDNS_MAX_LINELEN, line_nr) == -1) {
		goto error;
	}
	i = ldns_b64_pton((const char*)d, buf, ldns_b64_ntop_calculate_size(strlen(d)));
	dsa->pub_key = BN_bin2bn((const char unsigned*)buf, i, NULL);
	if (!dsa->pub_key) {
		goto error;
	}
#endif /* splint */

	LDNS_FREE(buf);
	LDNS_FREE(d);

	return dsa;

error:
	LDNS_FREE(d);
	LDNS_FREE(buf);
        DSA_free(dsa);
	return NULL;
}

unsigned char *
ldns_key_new_frm_fp_hmac(FILE *f, size_t *hmac_size)
{
	return ldns_key_new_frm_fp_hmac_l(f, NULL, hmac_size);
}

unsigned char *
ldns_key_new_frm_fp_hmac_l( FILE *f
			  , ATTR_UNUSED(int *line_nr)
			  , size_t *hmac_size
			  )
{
	size_t i, bufsz;
	char d[LDNS_MAX_LINELEN];
	unsigned char *buf = NULL;

	if (ldns_fget_keyword_data_l(f, "Key", ": ", d, "\n", LDNS_MAX_LINELEN, line_nr) == -1) {
		goto error;
	}
	bufsz = ldns_b64_ntop_calculate_size(strlen(d));
	buf = LDNS_XMALLOC(unsigned char, bufsz);
	i = (size_t) ldns_b64_pton((const char*)d, buf, bufsz);

	*hmac_size = i;
	return buf;

	error:
	LDNS_FREE(buf);
	*hmac_size = 0;
	return NULL;
}
#endif /* HAVE_SSL */

#ifdef USE_GOST
static EVP_PKEY*
ldns_gen_gost_key(void)
{
	EVP_PKEY_CTX* ctx;
	EVP_PKEY* p = NULL;
	int gost_id = ldns_key_EVP_load_gost_id();
	if(!gost_id)
		return NULL;
	ctx = EVP_PKEY_CTX_new_id(gost_id, NULL);
	if(!ctx) {
		/* the id should be available now */
		return NULL;
	}
	if(EVP_PKEY_CTX_ctrl_str(ctx, "paramset", "A") <= 0) {
		/* cannot set paramset */
		EVP_PKEY_CTX_free(ctx);
		return NULL;
	}

	if(EVP_PKEY_keygen_init(ctx) <= 0) {
		EVP_PKEY_CTX_free(ctx);
		return NULL;
	}
	if(EVP_PKEY_keygen(ctx, &p) <= 0) {
		EVP_PKEY_free(p);
		EVP_PKEY_CTX_free(ctx);
		return NULL;
	}
	EVP_PKEY_CTX_free(ctx);
	return p;
}
#endif

ldns_key *
ldns_key_new_frm_algorithm(ldns_signing_algorithm alg, uint16_t size)
{
	ldns_key *k;
#ifdef HAVE_SSL
	DSA *d;
	RSA *r;
#  ifdef USE_ECDSA
        EC_KEY *ec = NULL;
#  endif
#else
	int i;
	uint16_t offset = 0;
#endif
	unsigned char *hmac;

	k = ldns_key_new();
	if (!k) {
		return NULL;
	}
	switch(alg) {
		case LDNS_SIGN_RSAMD5:
		case LDNS_SIGN_RSASHA1:
		case LDNS_SIGN_RSASHA1_NSEC3:
		case LDNS_SIGN_RSASHA256:
		case LDNS_SIGN_RSASHA512:
#ifdef HAVE_SSL
			r = RSA_generate_key((int)size, RSA_F4, NULL, NULL);
                        if(!r) {
				ldns_key_free(k);
				return NULL;
			}
			if (RSA_check_key(r) != 1) {
				ldns_key_free(k);
				return NULL;
			}
			ldns_key_set_rsa_key(k, r);
			RSA_free(r);
#endif /* HAVE_SSL */
			break;
		case LDNS_SIGN_DSA:
		case LDNS_SIGN_DSA_NSEC3:
#ifdef HAVE_SSL
			d = DSA_generate_parameters((int)size, NULL, 0, NULL, NULL, NULL, NULL);
			if (!d) {
				ldns_key_free(k);
				return NULL;
			}
			if (DSA_generate_key(d) != 1) {
				ldns_key_free(k);
				return NULL;
			}
			ldns_key_set_dsa_key(k, d);
			DSA_free(d);
#endif /* HAVE_SSL */
			break;
		case LDNS_SIGN_HMACMD5:
		case LDNS_SIGN_HMACSHA1:
		case LDNS_SIGN_HMACSHA256:
#ifdef HAVE_SSL
#ifndef S_SPLINT_S
			k->_key.key = NULL;
#endif /* splint */
#endif /* HAVE_SSL */
			size = size / 8;
			ldns_key_set_hmac_size(k, size);

			hmac = LDNS_XMALLOC(unsigned char, size);
                        if(!hmac) {
				ldns_key_free(k);
				return NULL;
                        }
#ifdef HAVE_SSL
			if (RAND_bytes(hmac, (int) size) != 1) {
				LDNS_FREE(hmac);
				ldns_key_free(k);
				return NULL;
			}
#else
			while (offset + sizeof(i) < size) {
			  i = random();
			  memcpy(&hmac[offset], &i, sizeof(i));
			  offset += sizeof(i);
			}
			if (offset < size) {
			  i = random();
			  memcpy(&hmac[offset], &i, size - offset);
			}
#endif /* HAVE_SSL */
			ldns_key_set_hmac_key(k, hmac);

			ldns_key_set_flags(k, 0);
			break;
		case LDNS_SIGN_ECC_GOST:
#if defined(HAVE_SSL) && defined(USE_GOST)
			ldns_key_set_evp_key(k, ldns_gen_gost_key());
#ifndef S_SPLINT_S
                        if(!k->_key.key) {
                                ldns_key_free(k);
                                return NULL;
                        }
#endif /* splint */
#else
			ldns_key_free(k);
			return NULL;
#endif /* HAVE_SSL and USE_GOST */
                        break;
                case LDNS_SIGN_ECDSAP256SHA256:
                case LDNS_SIGN_ECDSAP384SHA384:
#ifdef USE_ECDSA
                        if(alg == LDNS_SIGN_ECDSAP256SHA256)
                                ec = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
                        else if(alg == LDNS_SIGN_ECDSAP384SHA384)
                                ec = EC_KEY_new_by_curve_name(NID_secp384r1);
                        if(!ec) {
                                ldns_key_free(k);
                                return NULL;
                        }
                        if(!EC_KEY_generate_key(ec)) {
                                ldns_key_free(k);
                                EC_KEY_free(ec);
                                return NULL;
                        }
#ifndef S_SPLINT_S
                        k->_key.key = EVP_PKEY_new();
                        if(!k->_key.key) {
                                ldns_key_free(k);
                                EC_KEY_free(ec);
                                return NULL;
                        }
                        if (!EVP_PKEY_assign_EC_KEY(k->_key.key, ec)) {
                                ldns_key_free(k);
                                EC_KEY_free(ec);
                                return NULL;
			}
#endif /* splint */
#else
			ldns_key_free(k);
			return NULL;
#endif /* ECDSA */
			break;
	}
	ldns_key_set_algorithm(k, alg);
	return k;
}

void
ldns_key_print(FILE *output, const ldns_key *k)
{
	char *str = ldns_key2str(k);
	if (str) {
                fprintf(output, "%s", str);
        } else {
                fprintf(output, "Unable to convert private key to string\n");
        }
        LDNS_FREE(str);
}


void
ldns_key_set_algorithm(ldns_key *k, ldns_signing_algorithm l)
{
	k->_alg = l;
}

void
ldns_key_set_flags(ldns_key *k, uint16_t f)
{
	k->_extra.dnssec.flags = f;
}

#ifdef HAVE_SSL
#ifndef S_SPLINT_S
void
ldns_key_set_evp_key(ldns_key *k, EVP_PKEY *e)
{
	k->_key.key = e;
}

void
ldns_key_set_rsa_key(ldns_key *k, RSA *r)
{
	EVP_PKEY *key = EVP_PKEY_new();
	EVP_PKEY_set1_RSA(key, r);
	k->_key.key = key;
}

void
ldns_key_set_dsa_key(ldns_key *k, DSA *d)
{
	EVP_PKEY *key = EVP_PKEY_new();
	EVP_PKEY_set1_DSA(key, d);
	k->_key.key  = key;
}

void
ldns_key_assign_rsa_key(ldns_key *k, RSA *r)
{
	EVP_PKEY *key = EVP_PKEY_new();
	EVP_PKEY_assign_RSA(key, r);
	k->_key.key = key;
}

void
ldns_key_assign_dsa_key(ldns_key *k, DSA *d)
{
	EVP_PKEY *key = EVP_PKEY_new();
	EVP_PKEY_assign_DSA(key, d);
	k->_key.key  = key;
}
#endif /* splint */
#endif /* HAVE_SSL */

void
ldns_key_set_hmac_key(ldns_key *k, unsigned char *hmac)
{
	k->_key.hmac.key = hmac;
}

void
ldns_key_set_hmac_size(ldns_key *k, size_t hmac_size)
{
	k->_key.hmac.size = hmac_size;
}

void
ldns_key_set_external_key(ldns_key *k, void *external_key)
{
	k->_key.external_key = external_key;
}

void
ldns_key_set_origttl(ldns_key *k, uint32_t t)
{
	k->_extra.dnssec.orig_ttl = t;
}

void
ldns_key_set_inception(ldns_key *k, uint32_t i)
{
	k->_extra.dnssec.inception = i;
}

void
ldns_key_set_expiration(ldns_key *k, uint32_t e)
{
	k->_extra.dnssec.expiration = e;
}

void
ldns_key_set_pubkey_owner(ldns_key *k, ldns_rdf *r)
{
	k->_pubkey_owner = r;
}

void
ldns_key_set_keytag(ldns_key *k, uint16_t tag)
{
	k->_extra.dnssec.keytag = tag;
}

/* read */
size_t
ldns_key_list_key_count(const ldns_key_list *key_list)
{
	        return key_list->_key_count;
}       

ldns_key *
ldns_key_list_key(const ldns_key_list *key, size_t nr)
{       
	if (nr < ldns_key_list_key_count(key)) {
		return key->_keys[nr];
	} else {
		return NULL;
	}
}

ldns_signing_algorithm
ldns_key_algorithm(const ldns_key *k) 
{
	return k->_alg;
}

void
ldns_key_set_use(ldns_key *k, bool v)
{
	if (k) {
		k->_use = v;
	}
}

bool
ldns_key_use(const ldns_key *k)
{
	if (k) {
		return k->_use;
	}
	return false;
}

#ifdef HAVE_SSL
#ifndef S_SPLINT_S
EVP_PKEY *
ldns_key_evp_key(const ldns_key *k)
{
	return k->_key.key;
}

RSA *
ldns_key_rsa_key(const ldns_key *k)
{
	if (k->_key.key) {
		return EVP_PKEY_get1_RSA(k->_key.key);
	} else {
		return NULL;
	}
}

DSA *
ldns_key_dsa_key(const ldns_key *k)
{
	if (k->_key.key) {
		return EVP_PKEY_get1_DSA(k->_key.key);
	} else {
		return NULL;
	}
}
#endif /* splint */
#endif /* HAVE_SSL */

unsigned char *
ldns_key_hmac_key(const ldns_key *k)
{
	if (k->_key.hmac.key) {
		return k->_key.hmac.key;
	} else {
		return NULL;
	}
}

size_t
ldns_key_hmac_size(const ldns_key *k)
{
	if (k->_key.hmac.size) {
		return k->_key.hmac.size;
	} else {
		return 0;
	}
}

void *
ldns_key_external_key(const ldns_key *k)
{
	return k->_key.external_key;
}

uint32_t
ldns_key_origttl(const ldns_key *k)
{
	return k->_extra.dnssec.orig_ttl;
}

uint16_t
ldns_key_flags(const ldns_key *k)
{
	return k->_extra.dnssec.flags;
}

uint32_t
ldns_key_inception(const ldns_key *k)
{
	return k->_extra.dnssec.inception;
}

uint32_t
ldns_key_expiration(const ldns_key *k)
{
	return k->_extra.dnssec.expiration;
}

uint16_t
ldns_key_keytag(const ldns_key *k)
{
	return k->_extra.dnssec.keytag;
}

ldns_rdf *
ldns_key_pubkey_owner(const ldns_key *k)
{
	return k->_pubkey_owner;
}

/* write */
void
ldns_key_list_set_use(ldns_key_list *keys, bool v)
{
	size_t i;

	for (i = 0; i < ldns_key_list_key_count(keys); i++) {
		ldns_key_set_use(ldns_key_list_key(keys, i), v);
	}
}

void            
ldns_key_list_set_key_count(ldns_key_list *key, size_t count)
{
	        key->_key_count = count;
}       

bool             
ldns_key_list_push_key(ldns_key_list *key_list, ldns_key *key)
{       
        size_t key_count;
        ldns_key **keys;

        key_count = ldns_key_list_key_count(key_list);

        /* grow the array */
        keys = LDNS_XREALLOC(
                key_list->_keys, ldns_key *, key_count + 1);
        if (!keys) {
                return false;
        }

        /* add the new member */
        key_list->_keys = keys;
        key_list->_keys[key_count] = key;

        ldns_key_list_set_key_count(key_list, key_count + 1);
        return true;
}

ldns_key *
ldns_key_list_pop_key(ldns_key_list *key_list)
{                               
        size_t key_count;
        ldns_key** a;
        ldns_key *pop;

	if (!key_list) {
		return NULL;
	}
        
        key_count = ldns_key_list_key_count(key_list);
        if (key_count == 0) {
                return NULL;
        }       
        
        pop = ldns_key_list_key(key_list, key_count);
        
        /* shrink the array */
        a = LDNS_XREALLOC(key_list->_keys, ldns_key *, key_count - 1);
        if(a) {
                key_list->_keys = a;
        }

        ldns_key_list_set_key_count(key_list, key_count - 1);

        return pop;
}       

#ifdef HAVE_SSL
#ifndef S_SPLINT_S
/* data pointer must be large enough (LDNS_MAX_KEYLEN) */
static bool
ldns_key_rsa2bin(unsigned char *data, RSA *k, uint16_t *size)
{
	int i,j;
	
	if (!k) {
		return false;
	}
	
	if (BN_num_bytes(k->e) <= 256) {
		/* normally only this path is executed (small factors are
		 * more common 
		 */
		data[0] = (unsigned char) BN_num_bytes(k->e);
		i = BN_bn2bin(k->e, data + 1);  
		j = BN_bn2bin(k->n, data + i + 1);
		*size = (uint16_t) i + j;
	} else if (BN_num_bytes(k->e) <= 65536) {
		data[0] = 0;
		/* BN_bn2bin does bigendian, _uint16 also */
		ldns_write_uint16(data + 1, (uint16_t) BN_num_bytes(k->e)); 

		BN_bn2bin(k->e, data + 3); 
		BN_bn2bin(k->n, data + 4 + BN_num_bytes(k->e));
		*size = (uint16_t) BN_num_bytes(k->n) + 6;
	} else {
		return false;
	}
	return true;
}

/* data pointer must be large enough (LDNS_MAX_KEYLEN) */
static bool
ldns_key_dsa2bin(unsigned char *data, DSA *k, uint16_t *size)
{
	uint8_t T;

	if (!k) {
		return false;
	}
	
	/* See RFC2536 */
	*size = (uint16_t)BN_num_bytes(k->p);
	T = (*size - 64) / 8;
	memcpy(data, &T, 1);

	if (T > 8) {
#ifdef STDERR_MSGS
		fprintf(stderr, "DSA key with T > 8 (ie. > 1024 bits)");
		fprintf(stderr, " not implemented\n");
#endif
		return false;
	}

	/* size = 64 + (T * 8); */
	data[0] = (unsigned char)T;
	BN_bn2bin(k->q, data + 1 ); 		/* 20 octects */
	BN_bn2bin(k->p, data + 21 ); 		/* offset octects */
	BN_bn2bin(k->g, data + 21 + *size); 	/* offset octets */
	BN_bn2bin(k->pub_key, data + 21 + *size + *size); /* offset octets */
	*size = 21 + (*size * 3);
	return true;
}

#ifdef USE_GOST
static bool
ldns_key_gost2bin(unsigned char* data, EVP_PKEY* k, uint16_t* size)
{
	int i;
	unsigned char* pp = NULL;
	if(i2d_PUBKEY(k, &pp) != 37 + 64) {
		/* expect 37 byte(ASN header) and 64 byte(X and Y) */
		CRYPTO_free(pp);
		return false;
	}
	/* omit ASN header */
	for(i=0; i<64; i++)
		data[i] = pp[i+37];
	CRYPTO_free(pp);
	*size = 64;
	return true;
}
#endif /* USE_GOST */
#endif /* splint */
#endif /* HAVE_SSL */

ldns_rr *
ldns_key2rr(const ldns_key *k)
{
	/* this function will convert a the keydata contained in
	 * rsa/dsa pointers to a DNSKEY rr. It will fill in as
	 * much as it can, but it does not know about key-flags
	 * for instance
	 */
	ldns_rr *pubkey;
	ldns_rdf *keybin;
	unsigned char *bin = NULL;
	uint16_t size = 0;
#ifdef HAVE_SSL
	RSA *rsa = NULL;
	DSA *dsa = NULL;
#endif /* HAVE_SSL */
#ifdef USE_ECDSA
        EC_KEY* ec;
#endif
	int internal_data = 0;

	if (!k) {
		return NULL;
	}
	pubkey = ldns_rr_new();

	switch (ldns_key_algorithm(k)) {
	case LDNS_SIGN_HMACMD5:
	case LDNS_SIGN_HMACSHA1:
	case LDNS_SIGN_HMACSHA256:
		ldns_rr_set_type(pubkey, LDNS_RR_TYPE_KEY);
        	break;
	default:
		ldns_rr_set_type(pubkey, LDNS_RR_TYPE_DNSKEY);
		break;
        }
	/* zero-th rdf - flags */
	ldns_rr_push_rdf(pubkey,
			ldns_native2rdf_int16(LDNS_RDF_TYPE_INT16,
				ldns_key_flags(k)));
	/* first - proto */
	ldns_rr_push_rdf(pubkey,
			ldns_native2rdf_int8(LDNS_RDF_TYPE_INT8, LDNS_DNSSEC_KEYPROTO));

	if (ldns_key_pubkey_owner(k)) {
		ldns_rr_set_owner(pubkey, ldns_rdf_clone(ldns_key_pubkey_owner(k)));
	}

	/* third - da algorithm */
	switch(ldns_key_algorithm(k)) {
		case LDNS_SIGN_RSAMD5:
		case LDNS_SIGN_RSASHA1:
		case LDNS_SIGN_RSASHA1_NSEC3:
		case LDNS_SIGN_RSASHA256:
		case LDNS_SIGN_RSASHA512:
			ldns_rr_push_rdf(pubkey,
						  ldns_native2rdf_int8(LDNS_RDF_TYPE_ALG, ldns_key_algorithm(k)));
#ifdef HAVE_SSL
			rsa =  ldns_key_rsa_key(k);
			if (rsa) {
				bin = LDNS_XMALLOC(unsigned char, LDNS_MAX_KEYLEN);
				if (!bin) {
                                        ldns_rr_free(pubkey);
					return NULL;
				}
				if (!ldns_key_rsa2bin(bin, rsa, &size)) {
		                        LDNS_FREE(bin);
                                        ldns_rr_free(pubkey);
					return NULL;
				}
				RSA_free(rsa);
				internal_data = 1;
			}
#endif
			size++;
			break;
		case LDNS_SIGN_DSA:
			ldns_rr_push_rdf(pubkey,
					ldns_native2rdf_int8(LDNS_RDF_TYPE_ALG, LDNS_DSA));
#ifdef HAVE_SSL
			dsa = ldns_key_dsa_key(k);
			if (dsa) {
				bin = LDNS_XMALLOC(unsigned char, LDNS_MAX_KEYLEN);
				if (!bin) {
                                        ldns_rr_free(pubkey);
					return NULL;
				}
				if (!ldns_key_dsa2bin(bin, dsa, &size)) {
		                        LDNS_FREE(bin);
                                        ldns_rr_free(pubkey);
					return NULL;
				}
				DSA_free(dsa);
				internal_data = 1;
			}
#endif /* HAVE_SSL */
			break;
		case LDNS_SIGN_DSA_NSEC3:
			ldns_rr_push_rdf(pubkey,
					ldns_native2rdf_int8(LDNS_RDF_TYPE_ALG, LDNS_DSA_NSEC3));
#ifdef HAVE_SSL
			dsa = ldns_key_dsa_key(k);
			if (dsa) {
				bin = LDNS_XMALLOC(unsigned char, LDNS_MAX_KEYLEN);
				if (!bin) {
                                        ldns_rr_free(pubkey);
					return NULL;
				}
				if (!ldns_key_dsa2bin(bin, dsa, &size)) {
		                        LDNS_FREE(bin);
                                        ldns_rr_free(pubkey);
					return NULL;
				}
				DSA_free(dsa);
				internal_data = 1;
			}
#endif /* HAVE_SSL */
			break;
		case LDNS_SIGN_ECC_GOST:
			ldns_rr_push_rdf(pubkey, ldns_native2rdf_int8(
				LDNS_RDF_TYPE_ALG, ldns_key_algorithm(k)));
#if defined(HAVE_SSL) && defined(USE_GOST)
			bin = LDNS_XMALLOC(unsigned char, LDNS_MAX_KEYLEN);
			if (!bin) {
                                ldns_rr_free(pubkey);
				return NULL;
                        }
#ifndef S_SPLINT_S
			if (!ldns_key_gost2bin(bin, k->_key.key, &size)) {
		                LDNS_FREE(bin);
                                ldns_rr_free(pubkey);
				return NULL;
			}
#endif /* splint */
			internal_data = 1;
#else
                        ldns_rr_free(pubkey);
			return NULL;
#endif /* HAVE_SSL and USE_GOST */
			break;
                case LDNS_SIGN_ECDSAP256SHA256:
                case LDNS_SIGN_ECDSAP384SHA384:
#ifdef USE_ECDSA
			ldns_rr_push_rdf(pubkey, ldns_native2rdf_int8(
				LDNS_RDF_TYPE_ALG, ldns_key_algorithm(k)));
                        bin = NULL;
#ifndef S_SPLINT_S
                        ec = EVP_PKEY_get1_EC_KEY(k->_key.key);
#endif
                        EC_KEY_set_conv_form(ec, POINT_CONVERSION_UNCOMPRESSED);
                        size = (uint16_t)i2o_ECPublicKey(ec, NULL);
                        if(!i2o_ECPublicKey(ec, &bin)) {
                                EC_KEY_free(ec);
                                ldns_rr_free(pubkey);
                                return NULL;
                        }
			if(size > 1) {
				/* move back one byte to shave off the 0x02
				 * 'uncompressed' indicator that openssl made
				 * Actually its 0x04 (from implementation).
				 */
				assert(bin[0] == POINT_CONVERSION_UNCOMPRESSED);
				size -= 1;
				memmove(bin, bin+1, size);
			}
                        /* down the reference count for ec, its still assigned
                         * to the pkey */
                        EC_KEY_free(ec);
			internal_data = 1;
#else
                        ldns_rr_free(pubkey);
			return NULL;
#endif /* ECDSA */
                        break;
		case LDNS_SIGN_HMACMD5:
		case LDNS_SIGN_HMACSHA1:
		case LDNS_SIGN_HMACSHA256:
			bin = LDNS_XMALLOC(unsigned char, ldns_key_hmac_size(k));
			if (!bin) {
                                ldns_rr_free(pubkey);
				return NULL;
			}
			ldns_rr_push_rdf(pubkey,
			                 ldns_native2rdf_int8(LDNS_RDF_TYPE_ALG,
			                 ldns_key_algorithm(k)));
			size = ldns_key_hmac_size(k);
			memcpy(bin, ldns_key_hmac_key(k), size);
			internal_data = 1;
			break;
	}
	/* fourth the key bin material */
	if (internal_data) {
		keybin = ldns_rdf_new_frm_data(LDNS_RDF_TYPE_B64, size, bin);
		LDNS_FREE(bin);
		ldns_rr_push_rdf(pubkey, keybin);
	}
	return pubkey;
}

void
ldns_key_free(ldns_key *key)
{
	LDNS_FREE(key);
}

void
ldns_key_deep_free(ldns_key *key)
{
	unsigned char* hmac;
	if (ldns_key_pubkey_owner(key)) {
		ldns_rdf_deep_free(ldns_key_pubkey_owner(key));
	}
#ifdef HAVE_SSL
	if (ldns_key_evp_key(key)) {
		EVP_PKEY_free(ldns_key_evp_key(key));
	}
#endif /* HAVE_SSL */
	if (ldns_key_hmac_key(key)) {
		hmac = ldns_key_hmac_key(key);
		LDNS_FREE(hmac);
	}
	LDNS_FREE(key);
}

void
ldns_key_list_free(ldns_key_list *key_list)
{
	size_t i;
	for (i = 0; i < ldns_key_list_key_count(key_list); i++) {
		ldns_key_deep_free(ldns_key_list_key(key_list, i));
	}
	LDNS_FREE(key_list->_keys);
	LDNS_FREE(key_list);
}

ldns_rr *
ldns_read_anchor_file(const char *filename)
{
	FILE *fp;
	/*char line[LDNS_MAX_PACKETLEN];*/
	char *line = LDNS_XMALLOC(char, LDNS_MAX_PACKETLEN);
	int c;
	size_t i = 0;
	ldns_rr *r;
	ldns_status status;
        if(!line) {
                return NULL;
        }

	fp = fopen(filename, "r");
	if (!fp) {
#ifdef STDERR_MSGS
		fprintf(stderr, "Unable to open %s: %s\n", filename, strerror(errno));
#endif
		LDNS_FREE(line);
		return NULL;
	}
	
	while ((c = fgetc(fp)) && i+1 < LDNS_MAX_PACKETLEN && c != EOF) {
		line[i] = c;
		i++;
	}
	line[i] = '\0';
	
	fclose(fp);
	
	if (i <= 0) {
#ifdef STDERR_MSGS
		fprintf(stderr, "nothing read from %s", filename);
#endif
		LDNS_FREE(line);
		return NULL;
	} else {
		status = ldns_rr_new_frm_str(&r, line, 0, NULL, NULL);
		if (status == LDNS_STATUS_OK && (ldns_rr_get_type(r) == LDNS_RR_TYPE_DNSKEY || ldns_rr_get_type(r) == LDNS_RR_TYPE_DS)) {
			LDNS_FREE(line);
			return r;
		} else {
#ifdef STDERR_MSGS
			fprintf(stderr, "Error creating DNSKEY or DS rr from %s: %s\n", filename, ldns_get_errorstr_by_id(status));
#endif
			LDNS_FREE(line);
			return NULL;
		}
	}
}

char *
ldns_key_get_file_base_name(ldns_key *key)
{
	ldns_buffer *buffer;
	char *file_base_name;
	
	buffer = ldns_buffer_new(255);
	ldns_buffer_printf(buffer, "K");
	(void)ldns_rdf2buffer_str_dname(buffer, ldns_key_pubkey_owner(key));
	ldns_buffer_printf(buffer,
	                   "+%03u+%05u",
			   ldns_key_algorithm(key),
			   ldns_key_keytag(key));
	file_base_name = ldns_buffer_export(buffer);
	ldns_buffer_free(buffer);
	return file_base_name;
}

int ldns_key_algo_supported(int algo)
{
	ldns_lookup_table *lt = ldns_signing_algorithms;
	while(lt->name) {
		if(lt->id == algo)
			return 1;
		lt++;
	}
	return 0;
}

ldns_signing_algorithm ldns_get_signing_algorithm_by_name(const char* name)
{
        /* list of (signing algorithm id, alias_name) */
        ldns_lookup_table aliases[] = {
                /* from bind dnssec-keygen */
                {LDNS_SIGN_HMACMD5, "HMAC-MD5"},
                {LDNS_SIGN_DSA_NSEC3, "NSEC3DSA"},
                {LDNS_SIGN_RSASHA1_NSEC3, "NSEC3RSASHA1"},
                /* old ldns usage, now RFC names */
                {LDNS_SIGN_DSA_NSEC3, "DSA_NSEC3" },
                {LDNS_SIGN_RSASHA1_NSEC3, "RSASHA1_NSEC3" },
#ifdef USE_GOST
                {LDNS_SIGN_ECC_GOST, "GOST"},
#endif
                /* compat with possible output */
                {LDNS_DH, "DH"},
                {LDNS_ECC, "ECC"},
                {LDNS_INDIRECT, "INDIRECT"},
                {LDNS_PRIVATEDNS, "PRIVATEDNS"},
                {LDNS_PRIVATEOID, "PRIVATEOID"},
                {0, NULL}};
        ldns_lookup_table* lt = ldns_signing_algorithms;
        while(lt->name) {
                if(strcasecmp(lt->name, name) == 0)
                        return lt->id;
                lt++;
        }
        lt = aliases;
        while(lt->name) {
                if(strcasecmp(lt->name, name) == 0)
                        return lt->id;
                lt++;
        }
        if(atoi(name) != 0)
                return atoi(name);
        return 0;
}
