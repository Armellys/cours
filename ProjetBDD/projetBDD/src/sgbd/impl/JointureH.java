package sgbd.impl;

import java.util.Hashtable;
import java.util.Vector;
import java.security.*;
import sgbd.operateurs.Jointure;
import sgbd.stockage.Nuplet;
import java.util.Arrays;

public class JointureH implements Jointure{

	public JointureH() {
		// TODO Auto-generated constructor stub
	}

	@Override
	public Nuplet[] jointure(Nuplet[] t1, Nuplet[] t2, int att1, int att2) {
		
		
		try {
			MessageDigest md = MessageDigest.getInstance("MD5");
			Hashtable<String,Vector<Nuplet>> tHash= new Hashtable<String,Vector<Nuplet>>();// Notre table de hachage
			Vector<Nuplet> res = new Vector<Nuplet>();// Vecteur pour sauvegarder notre réponse de jointure
			
			//******************* HACHAGE ************************//
			
			for(int i=0;i<t1.length;i++){
				byte[] b = {(byte)t1[i].getAtt(att1)};
				//récupération du byte de l'attribut 
				byte[] hash = md.digest(b);
				//création du hash avec MD5
				
				//On regarde si ce hash existe deja
				Vector<Nuplet> v = tHash.get(Arrays.toString(hash));
				//Si il existe pas
				if(v==null){
					Vector<Nuplet> vec = new Vector<Nuplet>();
					vec.add(t1[i]);
					//on crée un nouveau vecteur vide dans lequel on met notre Nuplet
					tHash.put(Arrays.toString(hash), vec);
					//on met le vecteur de Nuplet dans notre table /!\ on prend une string pour le hash sinon la comparaison ne se fait pas
				}
				//si le hash existait deja on ne cré pas de nouveau vecteur on ajoute à celui qu'on a récupéré
				else{
					v.add(t1[i]);
					tHash.put(Arrays.toString(hash), v);
				}
			}
			
			
			
			//************************* JOINTURE ****************************//
			//on va parcourire le 2ème tableau de Nuplet puis associé avec la table de hachage
			for(int i=0;i<t2.length;i++){
				byte[] b = {(byte)t2[i].getAtt(att2)};
				byte[] hash = md.digest(b);
				
				//on récupère le vecteur de Nuplet correspondant à notre hash
				Vector<Nuplet> v = tHash.get(Arrays.toString(hash));
				
				//si ce vecteur n'est pas null, on va faire la jointure entre les Nuplet du vecteur et celui de notre tableau
				if(v!=null){
				for(int j=0;j<v.size();j++){
					Nuplet n = v.elementAt(j);
					int a = n.size()+t2[i].size();
					byte[] bn = new byte[a];
					System.arraycopy(n.getValues(), 0, bn, 0, n.size());
					System.arraycopy(t2[i].getValues(), 0, bn, n.size(), t2[i].size());
					NupletInt ni = new NupletInt(bn);
					res.addElement(ni);
				}
				}
				
				
			}
			
			
			//*********************** RESULTAT **************************//
			
			//on récupère notre résultat
			Nuplet[] ret = new Nuplet[res.size()];
			for(int i=0;i<res.size();i++)
				ret[i] = res.elementAt(i);
			return ret;
			
			
			//System.out.println("table hache : "+tHash);
			
		
			
			
		} catch (NoSuchAlgorithmException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		
		
		return null;
	}

}