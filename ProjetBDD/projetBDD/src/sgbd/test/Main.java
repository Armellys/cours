package sgbd.test;

import sgbd.stockage.*;
import sgbd.impl.*;

public class Main {
	
	public static final int datasetSize = 100;
	public static final int nupletSize = 10;

	public static void main(String[] args){
		
		// Génération des données
		Nuplet[] tab = new NupletInt[datasetSize];
		for(int i=0;i<datasetSize;i++){
			tab[i] = new NupletInt(nupletSize);
			for(int j=0;j<nupletSize;j++){
				tab[i].putAtt(j, (byte)(j+i));
			}
		}

		// Implémentation avec Table
		
		System.out.println("------------------------------------------------");	
		System.out.println("Création d'une table");
		Table t = new TableInt("/tmp/table2", nupletSize);
		for(int i=0;i<datasetSize;i++){
			t.put(tab[i]);
			}
		System.out.println("Lecture d'une table");
		for(int i=0;i<datasetSize;i++){
			System.out.println(t.get(i).toString());
			}
		
		// Utilisation de getByAtt
		System.out.println("Test GetByAtt");
		Nuplet[] res = t.getByAtt(4, (byte)50);//renvoie les nuplets qui ont la valeur 50 au 4ème enregistrement
		for(Nuplet n : res){
			System.out.println(n.toString());
		}
		
		System.out.println("Test full Scan");
		Nuplet[] resScan = t.fullScan();//scan toute la table de donnée
		for(Nuplet n : resScan)
			System.out.println(n.toString());
		}
	
	
	

	}

