#include <string.h>
#include <iostream>
#include <map>
#include "functions.h"

using namespace std;

int main(int argc, char const *argv[])
{
	unsigned char *clef;										 // pointeur sur la clef de cryptage
	unsigned char *iv = (unsigned char *)"f7ez4hao92hcne64";	 // 16 char de long, 128 bits de valeur d'initialisation car 16*8
	unsigned char *password_plaintext;							 // pointeur sur le plaintext à encrypter
	unsigned char ciphertext[128];								 // buffer pour le ciphertext
	unsigned char Write_ciphertext[128];						 // buffer permettant le stockage dans la mémoire
	unsigned char Read_ciphertext[128];							 // buffer contenant le ciphertext lu à partir du fichier
	unsigned char plaintext[128];								 // buffer pour le ciphertext déchiffré
	int plaintext_len, ciphertext_len;							 // longueur des deux chaine de char
	bool Fichier_vide;											 // indique si le fichier est vide ou non
	string mot_de_passe;										 // mot de passe principal
	string mot_de_passe_hache;									 // mot de passe principal haché
	int mot_de_passe_hache_length = mot_de_passe_hache.length(); // longueur du mot de passe haché ??????
	string plaintext_affichage;									 // permet l'affichage à l'utilisateur

	// TODO : FONCTION DE PRÉSENTATION DU LOGICIEL !

	// * Demande du mot de passe principal à l'utilisateur

	cout << "Entrez votre mot de passe principal : ";
	getline(cin, mot_de_passe); // lire la ligne, stocker dans mot_de_passe
	cin.clear();				// clear l'entrée
	cout << "Le mot de passe principal entré est : " << mot_de_passe << endl;
	mot_de_passe_hache = sha256(mot_de_passe);		// hache le mot de passe
	clef = (unsigned char *)&mot_de_passe_hache[0]; // pointe sur l'adresse du début du string
	cout << "Le clef est : " << clef << endl;

	// Phase 1 : consulter les mots de passe (tous), aucun si aucun mot de passe
	// Phase 2 : modifier, ajouter des mots de passe
	// Phase 3 : crypter et sauver les mots de passe sous forme de ciphertext

	// * Lecture + (initialisation si nécessaire) du fichier .dat contenant le ciphertext

	Fichier_vide = read(Read_ciphertext); // true = aucun cipher, false = ciphers à déchiffrer

	// * Le fichier contient des ciphertext, on les décrypte

	if (!Fichier_vide)
	{
		ciphertext_len = read_ciphertext_len();										   // récupère la longueur du ciphertext
		plaintext_len = decrypt(Read_ciphertext, ciphertext_len, clef, iv, plaintext); // déchiffre le message
		plaintext[plaintext_len] = '\0';											   // marque la fin de la chaine
		printf("Le decrypted text est :\n");
		printf("%s\n", plaintext);

		plaintext_affichage = convertToString((char *)plaintext, plaintext_len); // conversion en string
		afficher_plaintext(plaintext_affichage);								 // affiche id et mots de passe
	}

	// Phase 2.1 : ajouter
	string nouveau_mot_de_passe;
	string nouveau_mot_de_passe_id;
	string continuer;
	int nouveau_mot_de_passe_length;
	bool nouvelle_saisie = true;
	map<string, string> mdp;

	if (Fichier_vide) // plaintext vide
	{
		cout << "Aucun mot de passe enregistré !" << endl;
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
			getline(cin, continuer);
			cin.clear();
			if (continuer != "yes")
			{
				nouvelle_saisie = false;
			}
		}

		nouveau_mot_de_passe.clear();
		cout << "[ ";
		for (auto &item : mdp)
		{
			cout << item.first << ":" << item.second << " ";
			nouveau_mot_de_passe.append(item.second);
			nouveau_mot_de_passe.append(" ");
		}
		cout << "]" << endl;
		cout << "La chaîne de nouveau mots de passe est : " << endl;
		cout << nouveau_mot_de_passe << endl;

		write_mdp_id(mdp); // sauve les id des mots de passe

		password_plaintext = (unsigned char *)&nouveau_mot_de_passe[0];
		cout << "password_plaintext vaut : " << password_plaintext << endl;
		ciphertext_len = encrypt(password_plaintext, strlen((char *)password_plaintext), clef, iv, ciphertext);
		write(ciphertext, Write_ciphertext);
		write_ciphertext_len(ciphertext_len);

		// plaintext contient alors le nouveau mot de passe à encrypter
	}
	else // des ciphertext sont déjà présents dans le fichier Ciphers.dat et enregistré dans plaintext
	{
		string res_add;
		cout << "Des mot de passe du fichier sont déjà sauvé dans le array plaintext : " << plaintext << endl;
		cout << "Que souhaitez vous faire ? reset/add" << endl;
		getline(cin, res_add);
		cin.clear();
		if (res_add == "reset")
		{
			cout << "Suppression des mots de passe, veuillez en entrer des nouveaux :" << endl;
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
				getline(cin, continuer);
				cin.clear();
				if (continuer != "yes")
				{
					nouvelle_saisie = false;
				}
			}

			nouveau_mot_de_passe.clear();
			cout << "[ ";
			for (auto &item : mdp)
			{
				cout << item.first << ":" << item.second << " ";
				nouveau_mot_de_passe.append(item.second);
				nouveau_mot_de_passe.append(" ");
			}
			cout << "]" << endl;
			cout << "La chaîne de nouveau mots de passe est : " << endl;
			cout << nouveau_mot_de_passe << endl;

			write_mdp_id(mdp); // sauve les id des mots de passe

			password_plaintext = (unsigned char *)&nouveau_mot_de_passe[0];
			cout << "password_plaintext vaut : " << password_plaintext << endl;
			ciphertext_len = encrypt(password_plaintext, strlen((char *)password_plaintext), clef, iv, ciphertext);
			write(ciphertext, Write_ciphertext);
			write_ciphertext_len(ciphertext_len);
		}
		if (res_add == "add")
		{
			string plaintext_to_string = convertToString((char *)plaintext, plaintext_len);
			cout << "la version en string est : " << plaintext_to_string << endl;
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
				getline(cin, continuer);
				cin.clear();
				if (continuer != "yes")
				{
					nouvelle_saisie = false;
				}
			}
			cout << "[ ";
			for (auto &item : mdp)
			{
				cout << item.first << ":" << item.second << " ";
				plaintext_to_string.append(item.second);
				plaintext_to_string.append(" ");
			}
			cout << "]" << endl;
			cout << "La chaîne de nouveau mots de passe est : " << endl;
			cout << plaintext_to_string << endl;

			add_mdp_id(mdp);

			password_plaintext = (unsigned char *)&plaintext_to_string[0];
			cout << "password_plaintext vaut : " << password_plaintext << endl;
			ciphertext_len = encrypt(password_plaintext, strlen((char *)password_plaintext), clef, iv, ciphertext);
			write(ciphertext, Write_ciphertext);
			write_ciphertext_len(ciphertext_len);
		}

		/*
		cout << "Entrez un nouveau mot de passe" << endl;
		getline(cin,nouveau_mot_de_passe);
		cout << "Le(s) mot(s) de passe entré(s) est : " << nouveau_mot_de_passe << endl;
		password_plaintext = (unsigned char *)&nouveau_mot_de_passe[0];
		cout << "password_plaintext vaut : " << password_plaintext << endl;
		ciphertext_len = encrypt (password_plaintext, strlen ((char *)password_plaintext), clef, iv, ciphertext);
		write(ciphertext, Write_ciphertext);
    	write_ciphertext_len(ciphertext_len);
		*/
	}

	// Si on veut chiffrer les mots de passe
	/*
	ciphertext_len = encrypt (password_plaintext, strlen ((char *)password_plaintext), clef, iv, ciphertext);
	write(ciphertext, Write_ciphertext);
    write_ciphertext_len(ciphertext_len);
	*/

	// Si on veut lire les mots de passe
	/*
	read(Read_ciphertext);
    ciphertext_len = read_ciphertext_len();
	plaintext_len = decrypt(Read_ciphertext , ciphertext_len, clef, iv, plaintext);
	plaintext[plaintext_len] = '\0';
	printf("Le decrypted text est :\n");
	printf("%s\n", plaintext);	
	*/
	return 0;
}
