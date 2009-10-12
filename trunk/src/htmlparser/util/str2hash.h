/*
 * =====================================================================================
 *
 *       Filename:  str2hash.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/12/2009 03:14:16 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenghua Dai (Zhenghua Dai), djx.zhenghua@gamil.com
 *        Company:  dzh
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class Str2hash
{
	private:
		typedef unsigned int (*hashFunc)(char* str);
		enum {HashEmpty=254, HashConflict=255};
		unsigned char Hash2ID[256] ;
		unsigned char Hash2IDC[256];
		unsigned char Hash2IDCList[256];
		char** allstr;
	public:
		Str2hash(char** allStr, int n){clean(); hash=hashSum; create(allStr,n);};
		Str2hash(char** allStr, unsigned char* preHash2ID, unsigned char* preHash2IDC, unsigned char* preHash2IDCList){ hash=hashSum; memcpy(Hash2ID, preHash2ID, 256);memcpy(Hash2IDC, preHash2IDC, 256);memcpy(Hash2IDCList, preHash2IDCList, 256);};
		void clean(){ memset(this, 0, sizeof(Str2hash));}
		void setHashFunc(hashFunc hf){hash = hf;}

	private:
		unsigned int (*hash)(char* str);
		void create(char** allStr, int n);

		static unsigned int hashSum(char* tag)
		{
			int v = 0;
			int len = strlen(tag);
			for(int i=0;i< len; i++){
				v += (tag[i]);
			}
			v += len;
			return v % 254;
		}

	public:
		unsigned int get(char* str);
		void print();
        void print2file(char*);
};

inline void Str2hash::create(char** allStr, int n)
{
	unsigned char tmp[256];
	allstr = allStr;	
	for(int i=0;i<256;i++) {
		Hash2ID[i] = HashEmpty;
		tmp[i] = HashEmpty;
	}
	for(int i=0;i<n;i++){
		if(allStr[i][0] == 0) continue;
		unsigned int h = hash(allStr[i]);
		if(Hash2ID[h]) 
			tmp[ Hash2ID[h] ] = i;
		Hash2ID[h] = i;
	}

	int index =0;
	for(int i=0; i<n;i++){
		if( tmp[i] == HashEmpty) continue;
		int cc= i;	
		unsigned int h = hash(allStr[i]);	
		Hash2ID[h] = HashConflict;
		Hash2IDC[h]=index;
		while(cc != HashEmpty) {
			Hash2IDCList[index++] = cc;
			cc = tmp[cc];
		}
		Hash2IDC[h+1] = index;
	}

}

inline unsigned int Str2hash::get(char *tag)
{
	unsigned char h = hash(tag);
	if(( Hash2ID[h] != HashEmpty ) && (Hash2ID[h] != HashConflict)){
		if(strcasecmp(tag,allstr[Hash2ID[h]]) == 0) 
			return Hash2ID[h];
	}else if(Hash2ID[h] != HashConflict){
		for(int i=Hash2ID[h]; i< Hash2ID[h+1];i++){
			if(strcasecmp(tag,allstr[Hash2IDCList[i]]) == 0) 
				return Hash2IDCList[i];
		}
	}
	return -1;
}

inline void Str2hash::print()
{
	for(int i=0;i<256;i++) {
		if( Hash2ID[i] != HashEmpty){
			printf("[%d %s:%d]\n", i, allstr[Hash2ID[i]], Hash2ID[i] );
		}
	}

	for(int i=0;i<256;i++) {
		printf("%d %d\n", i, Hash2IDCList[i]);
	}
}

inline void Str2hash::print2file(char *fn)
{
    int i;
    FILE* fp = fopen(fn, "w");
    fprintf(fp, "#ifndef __TAG_HASH__HEADER\n");
    fprintf(fp, "#define __TAG_HASH__HEADER\n");

    //! Hash2ID
    fprintf(fp,"unsigned char preHash2ID[]={\n");
    for(i=0;i<255;i++) {
        fprintf(fp, "%d, ", Hash2ID[i]);
    }
    fprintf(fp, "%d};\n", Hash2ID[i]);

    //! Hash2ID
    fprintf(fp,"unsigned char preHash2IDC[]={\n");
    for(i=0;i<255;i++) {
        fprintf(fp, "%d, ", Hash2IDC[i]);
    }
    fprintf(fp, "%d};\n", Hash2IDC[i]);

    //! Hash2ID
    fprintf(fp,"unsigned char preHash2IDCList[]={\n");
    for(i=0;i<255;i++) {
        fprintf(fp, "%d, ", Hash2IDCList[i]);
    }
    fprintf(fp, "%d};\n", Hash2IDCList[i]);
    
    fprintf(fp, "#endif");
    fclose(fp);
}
