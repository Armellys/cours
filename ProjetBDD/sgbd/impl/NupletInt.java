package sgbd.impl;
import sgbd.stockage.*;

public class NupletInt implements Nuplet{

	private byte[] values;
	
	public NupletInt(int size){
		this.values = new byte[size];
	}
	
	public NupletInt(byte[] tab){
		this.values = new byte[tab.length];
		for(int i=0;i<tab.length;i++){
			this.values[i] = tab[i];
		}
	}
	
	public Object getAtt(int i) {
		return this.values[i];
	}
	
	public int size(){
		return this.values.length;
	}
	
	public String toString(){
		String s = "";
		for(int i=0;i<this.size();i++){
			s+=this.values[i]+"\t";
		}
		return s;
	}

	@Override
	public void putAtt(int i, Object o) {
		this.values[i] = (byte) o;
	}

}
