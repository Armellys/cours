package sgbd.operateurs;

import sgbd.stockage.Nuplet;

public interface Jointure {
	public Nuplet[] jointure(Nuplet[] t1, Nuplet[] t2, int att1, int att2); // Jointure sur t1.att1 = t2.att2
}
