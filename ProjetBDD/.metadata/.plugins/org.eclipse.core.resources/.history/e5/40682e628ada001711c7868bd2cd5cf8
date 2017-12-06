package sgbd.impl;

import java.util.Vector;

import sgbd.operateurs.Projection;
import sgbd.stockage.Nuplet;

public class ProjectionInt implements Projection {

	public ProjectionInt() {
		// TODO Auto-generated constructor stub
	}

	@Override
	public Nuplet[] project(Nuplet[] t, int[] atts) {
		// TODO Auto-generated method stub
		Vector<Nuplet> vec = new Vector<Nuplet>();
		//NupletInt n = new NupletInt(atts.length);
		NupletInt[] n = new NupletInt[t.length];
		byte[] b = new byte[atts.length];
		for(int i =0;i<n.length;i++){
			for(int j =0;j<atts.length;j++){
				b[j]=(byte)t[i].getAtt(atts[j]);
			}
			n[i]=new NupletInt(b);
		}
		
		return n;
	}

}
