package sgbd.impl;

import java.util.Vector;

import sgbd.operateurs.Jointure;
import sgbd.stockage.Nuplet;

public class JointureS implements Jointure {
	
	public JointureS() {
		// TODO Auto-generated constructor stub
	}

	@Override
	public Nuplet[] jointure(Nuplet[] t1, Nuplet[] t2, int att1, int att2) {
		quickSort(t1,att1,0,t1.length - 1);
		quickSort(t2,att2,0,t2.length - 1);
		
		Vector<Nuplet> vec = new Vector<Nuplet>();
		int i=0, j=0;
		while(i<t1.length && j< t2.length){
				if(t1[i].getAtt(att1) == t2[j].getAtt(att2)){
					int a = t1[i].size()+t2[j].size();
					byte[] b = new byte[a];
					System.arraycopy(t1[i].getValues(), 0, b, 0, t1[i].size());
					System.arraycopy(t2[j].getValues(), 0, b, t1[i].size(), t2[j].size());
					NupletInt n = new NupletInt(b);
					vec.addElement(n);
					if(i+1<t1.length && (t1[i+1].getValues()[att1] == t2[j].getValues()[att2])){
						i++;
					}
					else{
						j++;
					}
				}
				else if (t1[i].getValues()[att1] < t2[j].getValues()[att2]){
					i++;
				}
				else if (t1[i].getValues()[att1] > t2[j].getValues()[att2]){
					j++;
				}
			
		}
		
		Nuplet[] ret = new Nuplet[vec.size()];
		for(int z=0;z<vec.size();z++)
			ret[z] = vec.elementAt(z);
		return ret;
	}
	
	private void quickSort (Nuplet[] t, int att, int low, int high){
		
		int i = low, j=high;
		byte pivot = t[low + (high-low)/2].getValues()[att];
		while (i <= j){
			while(t[i].getValues()[att] < pivot){
				i++;
			}
			while(t[j].getValues()[att] > pivot){
				j--;
			}
			if (i <= j){
				Nuplet temp = t[i];
				t[i]=t[j];
				t[j]=temp;
				i++;
				j--;
			}
		}
		if (low < j){
			quickSort(t,att,low,j);
		}
		if(i < high){
			quickSort(t,att,i, high);
		}
	}

}
