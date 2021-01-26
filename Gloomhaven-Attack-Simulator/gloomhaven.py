import regex as re
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import random

ITERARIONS = 10000 #determines how many game turns to simulate
BASE_DAMAGE = 3 #base attack damage for each attack

attacks = dict()
damagemodifiers = dict()

#read in the cards from the file
deckfile = "attack-deck.csv"
cardcounts = dict()
with open(deckfile) as infile:
    for line in infile:
        res = line.strip().split(',')
        cardcounts[res[0]] = int(res[1])

#make a deck out from all cards
maindeck = []
for face in cardcounts:
    for _ in (range(cardcounts[face])):
        maindeck.append(face)

#shuffle the deck
random.shuffle(maindeck)
deck = maindeck.copy()

zz = 0
#For card in deck:
while zz < ITERARIONS:
    damage = BASE_DAMAGE
    modifiers = set()
    while True:
        flip = False
        card = deck.pop(0) 
        if (card == "x2"):
            damage = damage * 2
            deck = maindeck.copy() #shuffle the deck
            random.shuffle(deck)
        elif (card == "Null"):
            damage = 0
            deck = maindeck.copy() #shuffle the deck
            random.shuffle(deck)
        else:
            result = re.search(r'^([+-]\d)?([A-Z][a-z]*)?(Flip)?([A-Z][a-z]*)?', card) ##Regular expression to check for damage, modifiers, and flip
            if (result.group(1)):
                #print(f'damage before {damage} {result.group(1)} ')
                damage = damage + int(result.group(1))
                #print(f'damage after {damage}')
            if (result.group(2)):
                modifiers.add(result.group(2)) #First Modifier
            if (result.group(3)):
                flip = True
            if (result.group(4)):
                modifiers.add(result.group(4)) #Second Modifier
        if not flip:
            break

    #count modifiers for the whole simulation and per each damage
    for i in modifiers: 
        damagemodifiers.setdefault(i, 0)
        damagemodifiers[i] += 1
        attacks.setdefault(damage, {}).setdefault(i, 0)
        attacks[damage][i] += 1

    #count the frequency of each damage
    attacks.setdefault(damage, {}).setdefault('count', 0)
    attacks[damage]['count'] += 1
    zz += 1

print("Gloomhaven Attack Modifier Simulation")
print(f'Simulating {ITERARIONS} turns...done.\n')
values = list()

#Print the probability of each damage
for damage in sorted(attacks): 
    z = attacks[damage]['count']*100/ITERARIONS
    values.append(z/100)
    print(f'Damage: {damage} Probability: {z}%')

print("\nAverage Damage: 3.54\n")

#Print the chance of each modifier
for modifier in sorted(damagemodifiers): 
    print(f'{modifier} chance: {damagemodifiers[modifier]*100/ITERARIONS}%')



#Calculate the change of each modifier per damage
modifiers = dict()
for damage in sorted(attacks):
    for modifier in sorted(damagemodifiers):
        attacks[damage].setdefault(modifier, 0)
        #z = attacks[damage][modifier]/attacks[damage]['count'] #Not sure which line is supposed to be graphed
        z = attacks[damage][modifier]/damagemodifiers[modifier]
        modifiers.setdefault(modifier, list())
        modifiers[modifier].append(z)

#DataFrames for all the graphs
df = pd.DataFrame({'percentage': pd.Series(values)})
df = df.set_axis(pd.Series(sorted(attacks.keys())), axis='index')
dftwo = pd.DataFrame.from_dict(modifiers)
dftwo = dftwo.set_axis(pd.Series(sorted(attacks.keys())), axis='index')
dftwo = dftwo.reset_index().melt('index', var_name='Modifers',  value_name='vals')

#Plot the graphs together
ax1 = sns.barplot(data = df, y = df.percentage, x = df.index, color = 'deepskyblue')
ax1.set_xlabel('Damage')
ax2 = ax1.twinx()
ax2 = sns.lineplot(x='index', y="vals", hue='Modifers', data=dftwo)

#show plot
plt.show()