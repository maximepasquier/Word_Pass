#include <string.h>
#include <iostream>
#include <map>
#include "functions.h"

using namespace std;

map<string, string> Saisie_mdp()
{
    bool nouvelle_saisie = true;
    string tmp_string;
    string nouveau_mot_de_passe;
    string nouveau_mot_de_passe_id;
    map<string, string> mdp;

    while (nouvelle_saisie)
    {
        nouveau_mot_de_passe.clear();
        nouveau_mot_de_passe_id.clear();
        cout << "Entrez un l'identifiant du nouveau mot de passe" << endl;
        getline(cin, nouveau_mot_de_passe_id);
        cin.clear();
        cout << "L'identifiant entré est : " << nouveau_mot_de_passe_id << endl;
        cout << "Entrez un nouveau mot de passe associé" << endl;
        getline(cin, nouveau_mot_de_passe);
        cin.clear();
        cout << "Le mot de passe entré est : " << nouveau_mot_de_passe << endl;
        mdp.insert(pair<string, string>(nouveau_mot_de_passe_id, nouveau_mot_de_passe));
        cout << "Mot de passe ajouté !" << endl;
        cout << "Souhaitez vous entrer un nouveau mot de passe ? yes/no" << endl;
        getline(cin, tmp_string);
        cin.clear();
        if (tmp_string != "yes")
        {
            nouvelle_saisie = false;
        }
    }
    return mdp;
}

int main(int argc, char const *argv[])
{
    map<string, string> mdp;
    mdp = Saisie_mdp();
    for (auto &item : mdp)
    {
        cout << item.first << ":" << item.second << " ";
    }
}