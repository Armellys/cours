package sgbd.impl;

import sgbd.stockage.Fichier;
import java.io.*;

public class FichierInt implements Fichier{

	private String fp;
	private RandomAccessFile f;
	private int nupletSize;
	private int currentLength;
	
	public FichierInt(String filePath, int nupletSize){
		this.fp = filePath;
		this.nupletSize = nupletSize;
	}

	public void store(int pos, Object o) {
		try {
			this.f = new RandomAccessFile(this.fp, "rw");
			//NupletInt n = (NupletInt) o;
			byte[] b = new byte[nupletSize];
			for(int i=0;i<nupletSize;i++)
				b[i] = (byte)((NupletInt) o).getAtt(i);
			this.f.seek(pos*nupletSize);
			this.f.write(b);
			this.f.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	public void deleteLine(int pos) {
		
		try {
		this.f = new RandomAccessFile(this.fp, "rw");
        int counter = 0, target = pos;
        long offset = 0, length = 0;
        while (f.readLine() != null) {
            counter++;
			if (counter == target)
                break; // Found target line's offset
            offset = f.getFilePointer();
        }

        length = f.getFilePointer() - offset;

        if (target > counter) {
            f.close();
            throw new IOException("No such line!");
        }

        byte[] buffer = new byte[4096];
        int read = -1; // will store byte reads from file.read()
        while ((read = f.read(buffer)) > -1){
            f.seek(f.getFilePointer() - read - length);
            f.write(buffer, 0, read);
            f.seek(f.getFilePointer() + length);
        }
        f.setLength(f.length() - length); //truncate by length
        f.close();
		}catch (Exception e) {
			e.printStackTrace();
		}
		
    }


	

	@Override
	public Object get(int pos) {
		try {
			this.f = new RandomAccessFile(this.fp, "rw");
			byte[] b = new byte[nupletSize];
			this.f.seek(pos*nupletSize);
			f.read(b);
			f.close();
			return new NupletInt(b);
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	}
	public int getPos(NupletInt n) {
		try {
			this.f = new RandomAccessFile(this.fp, "rw");
			int bol =0;
			int i;
			byte[] b = new byte[nupletSize];
			for(i=0; i<this.getCurrentSize();i=i+nupletSize){
				f.read(b);
				for (int j=0;j<b.length;j++){
					bol=0;
					if(b[j]!=n.getValues()[j]){
						bol=1;
						break;
					}
					
				}
				if(bol==0){
					break;
				}
			}	
			f.close();
			if(bol==0){
			return i/nupletSize;
			}
			else{
				return -1;
			}
		} catch (Exception e) {
			e.printStackTrace();
			return -1;
		}
	}

	@Override
	public long getCurrentSize() {
		return this.currentLength;
	}
	
	

}
