package sgbd.impl;

import java.util.Vector;

import sgbd.operateurs.Jointure;
import sgbd.stockage.Nuplet;

public class JointureInt implements Jointure {

	public JointureInt() {
		// TODO Auto-generated constructor stub
	}

	@Override
	public Nuplet[] jointure(Nuplet[] t1, Nuplet[] t2, int att1, int att2) {
		
		Vector<Nuplet> vec = new Vector<Nuplet>();
		for(int i=0;i<t1.length;i++){
			for(int j=0;j<t2.length;j++){
				if(t1[i].getAtt(att1) == t2[j].getAtt(att2)){
					int a = t1[i].size()+t2[j].size();
					byte[] b = new byte[a];
					System.arraycopy(t1[i].getValues(), 0, b, 0, t1[i].size());
					System.arraycopy(t2[j].getValues(), 0, b, t1[i].size(), t2[j].size());
					NupletInt n = new NupletInt(b);
					vec.addElement(n);
				}
			}
		}
		
		Nuplet[] ret = new Nuplet[vec.size()];
		for(int i=0;i<vec.size();i++)
			ret[i] = vec.elementAt(i);
		return ret;
	}

}
