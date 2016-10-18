//(C) Bigstep.com 
//Helix tests
#include <string.h> 
#include <stdio.h> 
#include <he.h>
#include <string.h>

#define DATASTORE "he://.//dev/sdc"

int main(int argc, char **argv) {

	he_t he;
	he_item item;

	if(argc!=2)	
	{
		printf("Syntax: %s he://.//dev/nvme0n1\n",argv[0]);
		return -1;
	}

	printf("opening %s\n",argv[1]);

	//open helium datastore
	he = he_open(argv[1], "DATASTORE", HE_O_CREATE|HE_O_VOLUME_CREATE, NULL); 
	if (!he) {
		he_perror("he_open"); return -1;
	}

	const char *key="test-key";
	const char *val="test-val";

	item.key=(void*)key;
	item.val=(void*)val;
	item.key_len=strlen(key);
	item.val_len=strlen(val);
	
	//insert a value
	if(he_insert(he, &item))
	{
		he_perror("he_insert");
	}


	//close helium datastore
	if (he_close(he)) {
		he_perror("he_close"); return -2;
	}
}
