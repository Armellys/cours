#include "dct.h"
#include "image.h"
#include "mtrand.h"
#include <vector>

using namespace std; 

#define C 10 
#define M 5000 
#define tsq vector<double>

// Voir https://www.sstic.org/media/SSTIC2009/SSTIC-actes/Le_tracage_de_traitres_en_multimedia/SSTIC2009-Article-Le_tracage_de_traitres_en_multimedia-furon.pdf

double rand01()
{
    const double d = 999999; 
    
    return double(mtrand()%(int(d)))/d;
}

#define CARRE(x) ((x)*(x))

tsq gen_secret(const int key, const int m)
{
    tsq p(m); 

    const double t = 10e-4;
    const double tt = asin(sqrt(t));
    
    mtsrand(key);
    
    //p = sin(tt + (pi/2-2*tt)*rand(m,1)).^ 2;
    for(int i=0; i<m; i++)
    {
        double r = rand01();
        
        p[i] = CARRE(sin(tt + (M_PI/2 - 2*tt)*r));
    }
    
    return p; 
}    

tsq gen_sequence(const int id, const tsq& p)
{
    const int m = p.size(); 
    
    tsq s(m); 
    
    mtsrand(id);
    
    for(int i=0; i<m; i++)
    {
        if(rand01()<p[i]) s[i]=1; else s[i]=0; 
    }

    return s;
}

double compute_score(const tsq& obs, const tsq& suspect, const tsq& p)
{
    const int m = obs.size(); 
    
    double score = 0;
    
    for(int i=0; i<m; i++)
    {
        if(obs[i]==0)
        {
            if(suspect[i]==0)
            {
                score += sqrt(p[i]/(1.0 - p[i])); 
            }
            else
            {
                score -= sqrt((1.0 - p[i])/p[i]);
            }
        }
        else
        {
            if(suspect[i]==0)
            {
                score -= sqrt(p[i]/(1.0 - p[i]));
            }
            else
            {
                score += sqrt((1.0 - p[i])/p[i]); 
            }
        }
    }
    
    // ... 
    
    return score; 
}

tsq collusion(const vector<tsq>& sequences)
{
    const int m = sequences[0].size(); 
    const int c = sequences.size(); 

    tsq col(m); 
    
    for(int i=0; i<m; i++)
    {
        col[i] = sequences[rand()%c][i];
    }
    
    return col; 
}     


int main(int argc, char** argv)
{
    tsq P = gen_secret(999, M);
    
    vector<tsq> colluders(C);
    
    // Génération des séquences des pirates 
    for(int i=0; i<C; i++)
    {
        colluders[i] = gen_sequence(i, P); 
    }

    tsq obs = collusion(colluders); 
    
    // Calcul des scores 
    for(int i=0; i<C; i++)
    {
        cout << "User ID " << i << " : score = " << compute_score(obs, colluders[i], P) << endl; 
    }
    
    for(int i=C; i<C + 20; i++)
    {
        cout << "User ID " << i << " : score = " << compute_score(obs, gen_sequence(i, P), P) << endl; 
    }


	return 1; 
}