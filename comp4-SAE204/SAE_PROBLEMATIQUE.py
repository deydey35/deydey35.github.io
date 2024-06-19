import numpy as np
import pandas as pd
from sklearn.linear_model import LinearRegression
import re

############### MISE EN FORME ###############

# Chemin vers fichier CSV
chemin_fichier_csv = '/home/pc/Documents/BUT_1A/SEMESTRE 2/SAE BDD/peuplement/VUE.csv'

# Charger le fichier CSV dans un DataFrame
CollegeDF = pd.read_csv(chemin_fichier_csv)

def countVoyelles(nom):
    vowels = re.findall(r'[aeiouAEIOU]', nom)
    return len(vowels)

# Appliquer la fonction countVoyelles à la colonne des noms d'établissements
CollegeDF['nb_voyelles'] = CollegeDF['nom_de_l_etablissement'].apply(countVoyelles)

# Transformer DataFrame en array
CollegeAR = CollegeDF.to_numpy()

# Afficher le résultat pour vérifier
print(CollegeAR)


############### CENTREDUIRE ###############

""" fonction centre reduire """
def CentrReduir(T):
    T = np.array(T, dtype=np.float64)
    (n, p) = T.shape
    res = np.zeros((n, p))
    TMoy = np.mean(T, axis=0)
    TEcart = np.std(T, axis=0)
    
    for j in range(p):
        res[:, j] = (T[:, j] - TMoy[j]) / TEcart[j]
    return res
            
CollegeARnum = CollegeAR[:,2:8]
CollegeAR_CR = CentrReduir(CollegeARnum)


############### REPRESENTATIONS GRAPHIQUES ###############

import matplotlib.pyplot as plt


"""ELEVES AVEC MENTION"""

# Calcul du nombre total d'élèves ayant eu une mention par nombre de voyelles
total_mentions = CollegeDF.groupby('nb_voyelles')[['nb_mentions_ab_g', 'nb_mentions_b_g', 'nb_mentions_tb_g']].sum()

# Ajout d'une colonne pour le total des mentions
total_mentions['total_mentions'] = total_mentions.sum(axis=1)

# Ajouter total_mentions à CollegeDF
CollegeDF = pd.merge(CollegeDF, total_mentions['total_mentions'], left_on='nb_voyelles', right_index=True)

#### Création du diagramme en bâtons ####

plt.figure(figsize=(10, 6))
total_mentions['total_mentions'].plot(kind='bar', color='lightgreen')
plt.title('Nombre total d\'élèves ayant eu une mention par nombre de voyelles')
plt.xlabel('Nombre de voyelles dans le nom de l\'établissement')
plt.ylabel('Nombre total d\'élèves ayant eu une mention')
plt.xticks(rotation=0)
plt.grid(axis='y', linestyle='--', alpha=0.7)
plt.show()


#### Création du diagramme en boîte à moustaches ####

plt.figure(figsize=(12, 8))

# Box plot pour les mentions AB, B et TB
plt.boxplot([CollegeDF['nb_mentions_ab_g'], CollegeDF['nb_mentions_b_g'], CollegeDF['nb_mentions_tb_g']],
            labels=['Mention AB', 'Mention B', 'Mention TB'], patch_artist=True,
            boxprops=dict(facecolor='lightblue', color='blue'),
            whiskerprops=dict(color='blue'),
            capprops=dict(color='blue'),
            medianprops=dict(color='red'))

# Ajouter des labels et un titre
plt.xlabel('Types de mentions', fontweight='bold')
plt.ylabel('Nombre de mentions', fontweight='bold')
plt.title('Distribution du nombre de mentions par type', fontweight='bold')

plt.show()



"""ELEVE EN DIFFICULTES"""

# Calcul du nombre total d'élèves ULIS et SEGPA par nombre de voyelles
ulis_segpa = CollegeDF.groupby('nb_voyelles')[['nombre_eleves_ulis', 'nombre_eleves_segpa']].sum()

# Largeur des barres
bar_width = 0.25
# Positions des barres pour chaque catégorie
indices = np.arange(len(ulis_segpa.index))

# Diagramme pour ULIS
plt.bar(indices - bar_width, ulis_segpa['nombre_eleves_ulis'], width=bar_width, color='lightblue', label='ULIS')
# Diagramme pour SEGPA
plt.bar(indices, ulis_segpa['nombre_eleves_segpa'], width=bar_width, color='lightcoral', label='SEGPA')

# Ajout des labels, titre, légende et personnalisation
plt.title('Nombre total d\'élèves ULIS, SEGPA par nombre de voyelles')
plt.xlabel('Nombre de voyelles dans le nom de l\'établissement')
plt.ylabel('Nombre total d\'élèves')
plt.xticks(indices, ulis_segpa.index)
plt.legend(title='Type d\'élève')
plt.grid(axis='y', linestyle='--', alpha=0.7)

# Affichage du diagramme
plt.tight_layout()
plt.show()


############### MATRICE DE COVARIANCE ###############

MatriceCov = np.cov(CollegeAR_CR,rowvar=False)
print(MatriceCov)


############### REGRESSION LINEAIRE ###############

from sklearn.metrics import r2_score

# Sélection des variables pour la régression linéaire
Y = CollegeAR[:, 7]  # nb_voyelles
X = CollegeAR[:, [2, 3, 5]]  # colonnes 1, 2 et 4 de CollegeAR sans l'index

# Initialisation du modèle de régression linéaire
model = LinearRegression()

# Entraînement du modèle
model.fit(X, Y)

# Affichage des coefficients (paramètres)
print('Coefficients :', model.coef_)
print('Intercept :', model.intercept_)


# Prédiction des valeurs de Y
Ypred = model.predict(X)

# Calcul du coefficient de corrélation multiple (R²)
r2 = r2_score(Y, Ypred)
print('Coefficient de corrélation multiple (R²) :', r2)







