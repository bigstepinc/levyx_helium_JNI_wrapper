#include "com_bigstep_Helium.h"
#include "he.h"
#include "errno.h"
 #include <string.h>

JNIEXPORT jint JNICALL Java_com_bigstep_Helium_open
  (JNIEnv *env, jobject jobj, jstring jdataset)
{

	char buffer[1024];

	const char *dataset = env->GetStringUTFChars(jdataset, NULL);

	he_t he= he_open(dataset,"dataset", HE_O_CREATE|HE_O_VOLUME_CREATE, NULL);


 	if(!he){
	 	sprintf(buffer, he_strerror(errno));
		(env)->ThrowNew((env)->FindClass("java/lang/Exception"), buffer);
	}

	(env)->ReleaseStringUTFChars(jdataset, dataset);	

	 jclass clazz = env->GetObjectClass(jobj);
	 jfieldID fid = env->GetFieldID(clazz,"he","J");
	 env->SetLongField(clazz, fid, reinterpret_cast<jlong>(he));	

	 return errno;
}

/*
 * Class:     Helium
 * Method:    he_insert
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT jint JNICALL Java_com_bigstep_Helium_insert
  (JNIEnv * env, jobject jobj, jstring jkey, jstring jval)
{
	 jclass clazz = env->GetObjectClass(jobj);
	 jfieldID fid = env->GetFieldID(clazz,"he","J");
	 he_t he = reinterpret_cast<he_t>(env->GetLongField(clazz, fid));	
	 const char *key = env->GetStringUTFChars(jkey, NULL);
	 const char *val = env->GetStringUTFChars(jval, NULL);

	 he_item item;
	 item.key = (void*)key;
	 item.key_len = strnlen(key, 1024);	
	 item.val = (void*)val;
	 item.val_len = strnlen(val, 1024);

	 if(he_insert(he, &item)){
		char buffer[1024];
	 	sprintf(buffer, he_strerror(errno));
		(env)->ThrowNew((env)->FindClass("java/lang/Exception"), buffer);
	}

	return errno;
}

JNIEXPORT jstring JNICALL Java_com_bigstep_Helium_lookup
  (JNIEnv * env, jobject jobj, jstring jkey)
{
	 jclass clazz = env->GetObjectClass(jobj);
	 jfieldID fid = env->GetFieldID(clazz,"he","J");
	 he_t he = reinterpret_cast<he_t>(env->GetLongField(clazz, fid));	
	 const char *key = env->GetStringUTFChars(jkey, NULL);
	 char val[1024];
	 he_item item;
	 item.key = (void*)key;
	 item.key_len = strnlen(key, 1024);	
	 item.val = (void*)val;
	 item.val_len = strnlen(val, 1024);

	 if(he_lookup(he, &item, 0, 1024)){
		char buffer[1024];
	 	sprintf(buffer, he_strerror(errno));
		(env)->ThrowNew((env)->FindClass("java/lang/Exception"), buffer);
	}

	val[item.val_len]=0;//null termination

	return env->NewStringUTF(val);
}

JNIEXPORT jboolean JNICALL Java_com_bigstep_Helium_exists
  (JNIEnv * env, jobject jobj, jstring jkey)
{
	 jclass clazz = env->GetObjectClass(jobj);
	 jfieldID fid = env->GetFieldID(clazz,"he","J");
	 he_t he = reinterpret_cast<he_t>(env->GetLongField(clazz, fid));	
	 const char *key = env->GetStringUTFChars(jkey, NULL);

	 he_item item;
	 item.key = (void*)key;
	 item.key_len = strnlen(key, 1024);	
	 item.val = NULL;
	 item.val_len = 0;

	 int ret=he_exists(he, &item);
 	 if(ret==HE_ERR_ITEM_NOT_FOUND)
		return static_cast<jboolean>(false);
	 if(ret)
	{
		char buffer[1024];
	 	sprintf(buffer, he_strerror(errno));
		(env)->ThrowNew((env)->FindClass("java/lang/Exception"), buffer);
	}
	
	return static_cast<jboolean>(true);		
}





/*
 * Class:     Helium
 * Method:    destroy
 * Signature: ()V
 */
JNIEXPORT jint JNICALL Java_com_bigstep_Helium_close
  (JNIEnv *env, jobject jobj)
{
	 jclass clazz = env->GetObjectClass(jobj);
	 jfieldID fid = env->GetFieldID(clazz,"he","J");
	 he_t he = reinterpret_cast<he_t>(env->GetLongField(clazz, fid));	
	 if(he_close(he)){
		
		char buffer[1024];
	 	sprintf(buffer, he_strerror(errno));
		(env)->ThrowNew((env)->FindClass("java/lang/Exception"), buffer);
	}

	return errno;
}


