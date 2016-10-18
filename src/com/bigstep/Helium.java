
/* Written By Cosmin Pintoiu
*  Bigstep.com
*  Distrubuted under the same license as apache jmeter itself.
* http://www.apache.org/licenses/LICENSE-2.0
*
*/
package com.bigstep;

import java.io.IOException;
import cz.adamh.utils.NativeUtils;
import java.io.File;
import java.io.Serializable;


public class Helium {


	private long he;

	static{
		try {
		System.load(System.getProperty("user.dir")+"/libhelium.so");
  		 } catch (UnsatisfiedLinkError e) {
      		 try {
          	 NativeUtils.loadLibraryFromJar("/libhelium.so"); // during runtime. .DLL within .JAR
      		 	} catch (IOException e1) {
           			throw new RuntimeException(e1);
       				}
   			}
	}

	public native int open(String datastore);
	public native int insert(String key, String val);
	public native boolean exists(String key);
	public native String lookup(String key);
	public native int close();

	public static void main(String[] args){
		if(args.length!=3){
			System.err.println("Syntax: <datastore> <key> <value>\n Example: he://.//dev/nvme0n1 testkey testval");
			System.exit(-1);
		}


		String datastore = args[0];
		String key = args[1];
		String val = args[2];

		Helium he= new Helium();

		he.open(datastore);
		
		//insert if not exists
		if(!he.exists(key))
			he.insert(key,val);

		//lookup	
		String lval = he.lookup(key);

		if(!lval.equals(val))
			System.err.println("Test failed.key='"+key+"' val='"+lval+"'");

		he.close();
	}
}
