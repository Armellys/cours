package sgbd.operateurs;

import sgbd.stockage.Nuplet;

public interface Restriction {
	
	public Nuplet[] egalite(Nuplet[] t, int att, Object v); // Retourne les Nuplets de la table t dont l'attribut att est égal à la valeur v
	public Nuplet[] superieur(Nuplet[] t, int att, Object v);// Retourne les Nuplets de la table t dont l'attribut att est supérieur ou égal à la valeur v
	public Nuplet[] inferieur(Nuplet[] t, int att, Object v);// Retourne les Nuplets de la table t dont l'attribut att est inferieur ou égal à la valeur v

}
