package sgbd.impl;

import java.util.Vector;

import sgbd.operateurs.Restriction;
import sgbd.stockage.Nuplet;

public class RestrictionInt implements Restriction {

	public RestrictionInt() {
		// TODO Auto-generated constructor stub
	}

	@Override
	public Nuplet[] egalite(Nuplet[] t, int att, Object v) {
		
		Vector<Nuplet> vec = new Vector<Nuplet>();
		for(int i=0;i<t.length;i++){
				if ((byte)t[i].getAtt(att)==(byte)v){
					vec.addElement(t[i]);
				}
			
		}
		Nuplet[] ret = new Nuplet[vec.size()];
		for(int i=0;i<vec.size();i++)
			ret[i] = vec.elementAt(i);
		return ret;
	}

	@Override
	public Nuplet[] superieur(Nuplet[] t, int att, Object v) {
		
		Vector<Nuplet> vec = new Vector<Nuplet>();
		for(int i=0;i<t.length;i++){
				if ((byte)t[i].getAtt(att)>=(byte)v){
					vec.addElement(t[i]);
				}
			
		}
		Nuplet[] ret = new Nuplet[vec.size()];
		for(int i=0;i<vec.size();i++)
			ret[i] = vec.elementAt(i);
		return ret;
		// TODO Auto-generated method stub
		
	}

	@Override
	public Nuplet[] inferieur(Nuplet[] t, int att, Object v) {
		// TODO Auto-generated method stub
		Vector<Nuplet> vec = new Vector<Nuplet>();
		for(int i=0;i<t.length;i++){
			if ((byte)t[i].getAtt(att)<=(byte)v){
				vec.addElement(t[i]);
			}
		
	}
	Nuplet[] ret = new Nuplet[vec.size()];
	for(int i=0;i<vec.size();i++)
		ret[i] = vec.elementAt(i);
	return ret;
	}

}
