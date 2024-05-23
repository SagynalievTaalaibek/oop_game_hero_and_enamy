#include "imports.h"

using namespace std;

class Weapon {
    string name;
    int damage;

public:
    Weapon(string n, int dmg) : name(n), damage(dmg) {}

    int getDamage() const {
        return damage;
    }

    string getName() const {
        return name;
    }
};

class Armor {
    string name;
    int defense; 

public:
    Armor(string n, int def) : name(n), defense(def) {}

    int getDefense() const {
        return defense;
    }

    string getName() const {
        return name;
    }
};

class Potion {
    string name;
    int healing;

public:
    Potion(string n, int heal) : name(n), healing(heal) {}

    int getHealing() const {
        return healing;
    }

    string getName() const {
        return name;
    }
};

class Inventory {
public:
    vector<string> items;

    void addItem(const string& item) {
        items.push_back(item);
    }

    void removeItem(const string& item) {
        auto it = find(items.begin(), items.end(), item);
        if (it != items.end()) {
            items.erase(it);
        }
    }

    void displayItems() const {
        cout << "Inventory:" << endl;
        for (size_t i = 0; i < items.size(); ++i) {
            cout << i + 1 << " - " << items[i] << endl;
        }
    }

    string getItem(size_t index) const {
        if (index < items.size()) {
            return items[index];
        }
        return "";
    }
};

class Dice {
public:
    static int randomInt(int min, int max) {
        return rand() % (max - min + 1) + min;
    }

    static int d20() {
        return randomInt(1, 20);
    }

    static int d6() {
        return randomInt(1, 6);
    }

    static int d4() {
        return randomInt(1, 4);
    }
};

class Character {
public:
    string name;
    int hitPoints;
    int armorClass;
    int attackBonus;
    Inventory inventory;
    Weapon* equippedWeapon;
    Armor* equippedArmor;

    Character(string n, int hp, int ac, int ab) : name(n), hitPoints(hp), armorClass(ac), attackBonus(ab), equippedWeapon(nullptr), equippedArmor(nullptr) {}

    void changeHP(int damage) {
        if (damage >= hitPoints) {
            hitPoints = 0;
        }
        else {
            hitPoints -= damage;
        }
    }

    void healHP(int amount) {
        hitPoints += amount;
    }

    string getStatus() const {
        return name + ": " + to_string(hitPoints) + " HP";
    }

    virtual void attack(Character& target) = 0;

    void showInventory() const {
        inventory.displayItems();
    }

    void addItemToInventory(const string& item) {
        inventory.addItem(item);
    }

    void removeItemFromInventory(const string& item) {
        inventory.removeItem(item);
    }

    void equipWeapon(Weapon* weapon) {
        equippedWeapon = weapon;
    }

    void unequipWeapon() {
        equippedWeapon = nullptr;
    }

    void equipArmor(Armor* armor) {
        equippedArmor = armor;
    }

    void unequipArmor() {
        equippedArmor = nullptr;
    }

    int getAttackDamage() const {
        return attackBonus + (equippedWeapon ? equippedWeapon->getDamage() : 0);
    }

    int getTotalDefense() const {
        return armorClass + (equippedArmor ? equippedArmor->getDefense() : 0);
    }

    void usePotion(Potion* potion) {
        healHP(potion->getHealing());
        removeItemFromInventory(potion->getName());
        cout << name << " uses " << potion->getName() << " and heals " << potion->getHealing() << " HP\n" << getStatus() << endl;
    }

    void useArmor(Armor* armor) {
        if (equippedArmor) {
            cout << "Already wearing armor: " << equippedArmor->getName() << endl;
        }
        else {
            equippedArmor = armor;
            cout << name << " equips " << armor->getName() << endl;
        }
    }

    void useWeapon(Weapon* weapon) {
        if (equippedWeapon) {
            cout << "Already equipped with weapon: " << equippedWeapon->getName() << endl;
        }
        else {
            equippedWeapon = weapon;
            cout << name << " equips " << weapon->getName() << endl;
        }
    }

    void useItemFromInventory(size_t index) {
        string item = inventory.getItem(index);
        if (item == "Health Potion") {
            Potion healthPotion("Health Potion", 10);
            usePotion(&healthPotion);
        }
        else if (item == "Sword") {
            Weapon sword("Sword", 5);
            useWeapon(&sword);

            attackBonus *= 2;
            cout << "Hero's attack increased to: " << getAttackDamage() << endl;
        }

        else if (item == "Shield") {
            Armor shield("Shield", 2);
            useArmor(&shield);
        }
        else {
            cout << "Cannot use " << item << endl;
        }
    }
};

class Hero : public Character {
public:
    int magicSlots;
    vector<string> bard;

    Hero(string n, int hp, int ac, int ab, int ms) : Character(n, hp, ac, ab), magicSlots(ms) {
        bard = {
            "Хватит валять дурака, пора уже тушить пожар в этой программе.",
            "Мне сказали, что я как маг - всегда готов применить заклинание. Только вместо огня у меня аккорды!",
            "Скелеты, гоблины, жуткие твари... Кажется, мой концерт в подземелье начался!",
            "Трус умирает сто раз. Мужественный человек – лишь однажды.",
            "Людям для жизни необходимы три вещи: еда, питье и сплетни.",
            "Когда бросаете кости на успех, не забывайте добавить немного моей музыки - она всегда приносит удачу (или хотя бы улыбку)!"
        };
    }

    void attack(Character& target) override {
        int heroAttack = Dice::d20() + attackBonus;
        if (heroAttack > target.getTotalDefense()) {
            int damage = Dice::d6() + Dice::d6() + (equippedWeapon ? equippedWeapon->getDamage() : 0);
            target.changeHP(damage);
            cout << name << " attacks " << target.name << " for " << damage << " damage\n" << target.getStatus() << endl;
        }
        else {
            cout << "FAIL!!! " << name << " did not attack" << endl;
        }
    }

    int getMagicMissileDamage() {
        if (magicSlots <= 0) {
            return 0;
        }
        else {
            magicSlots--;
            return Dice::d4() + Dice::d4() + Dice::d4() + 3;
        }
    }

    string listenBard() const {
        int randomIndex = Dice::randomInt(0, bard.size() - 1);
        return bard[randomIndex];
    }
};

class Enemy : public Character {
public:
    Enemy(string n, int hp, int ac, int ab) : Character(n, hp, ac, ab) {}

    void attack(Character& target) override {
        int enemyAttack = Dice::d20() + attackBonus;
        if (enemyAttack > target.getTotalDefense()) {
            int damage = Dice::d6() + (equippedWeapon ? equippedWeapon->getDamage() : 0);
            target.changeHP(damage);
            cout << name << " attacks " << target.name << " for " << damage << " damage\n" << target.getStatus() << endl;
        }
        else {
            cout << "FAIL!!! " << name << " did not attack" << endl;
        }
    }
};

class Warrior : public Hero {
public:
    Warrior() : Hero("Warrior", 30, 14, 6, 1) {}

    void specialAbility(Character& target) {
        int damage = Dice::d6() + 2 + (equippedWeapon ? equippedWeapon->getDamage() : 0);
        target.changeHP(damage);
        cout << name << " uses special ability on " << target.name << " for " << damage << " damage\n" << target.getStatus() << endl;
    }
};

class Mage : public Hero {
public:
    Mage() : Hero("Mage", 20, 10, 4, 3) {}

    void castSpell(Character& target) {
        if (magicSlots > 0) {
            int damage = Dice::d6() + Dice::d6() + Dice::d6();
            target.changeHP(damage);
            magicSlots--;
            cout << name << " casts spell on " << target.name << " for " << damage << " damage\n" << target.getStatus() << endl;
        }
        else {
            cout << "No more spell slots!" << endl;
        }
    }
};

class Goblin : public Enemy {
public:
    Goblin() : Enemy("Goblin", 15, 10, 3) {}
};

class Dragon : public Enemy {
public:
    Dragon() : Enemy("Dragon", 50, 18, 8) {}

    void breathFire(Character& target) {
        int damage = Dice::d6() + Dice::d6() + Dice::d6();
        target.changeHP(damage);
        cout << name << " breathes fire on " << target.name << " for " << damage << " damage\n" << target.getStatus() << endl;
    }
};


class Batman : public Hero {
public:
    Batman() : Hero("Batman", 25, 12, 5, 2) {}

    void useGadget(Character& target) {
        int damage = Dice::d6() + Dice::d6();
        target.changeHP(damage);
        cout << name << " uses gadget on " << target.name << " for " << damage << " damage\n" << target.getStatus() << endl;
    }
};


class Joker : public Enemy {
public:
    Joker() : Enemy("Joker", 30, 15, 4) {}

    void useTrick(Character& target) {
        int damage = Dice::d4() + Dice::d4();
        target.changeHP(damage);
        cout << name << " uses trick on " << target.name << " for " << damage << " damage\n" << target.getStatus() << endl;
    }
};

class Game {
public:
    static void startGame() {
        srand(static_cast<unsigned int>(time(0)));

        vector<Hero*> heroes = { new Warrior(), new Mage(), new Batman() };
        vector<Enemy*> enemies = { new Goblin(), new Dragon(), new Joker() };

        int heroChoice;
        cout << "Choose your hero:\n1 - Warrior\n2 - Mage\n3 - Batman" << endl;
        cin >> heroChoice;
        Hero* hero = heroes[heroChoice - 1];

        int enemyChoice;
        cout << "Choose your enemy:\n1 - Goblin\n2 - Dragon\n3 - Joker" << endl;
        cin >> enemyChoice;
        Enemy* enemy = enemies[enemyChoice - 1];

        Weapon sword("Sword", 5);
        Armor shield("Shield", 2);
        Armor leatherArmor("Leather Armor", 3);
        Potion healthPotion("Health Potion", 10);

        hero->addItemToInventory("Sword");
        hero->addItemToInventory("Shield");
        hero->addItemToInventory("Health Potion");

        enemy->addItemToInventory("Dagger");
        enemy->addItemToInventory("Leather Armor");

        hero->equipWeapon(&sword);
        hero->equipArmor(&shield);

        enemy->equipArmor(&leatherArmor);

        while (true) {
            int userChoice;
            cout << "1 - Attack\n2 - Use the spell Magic Arrow\n3 - Listen to the bard\n4 - Show Inventory\n5 - Use Item from Inventory\n6 - Run away" << endl;
            cin >> userChoice;

            if (userChoice == 1) {
                hero->attack(*enemy);
            }
            else if (userChoice == 2) {
                int magicDamage = hero->getMagicMissileDamage();
                if (magicDamage == 0) {
                    cout << "No more Magic Arrow" << endl;
                    continue;
                }
                else {
                    enemy->changeHP(magicDamage);
                    cout << hero->name << " attacks " << enemy->name << " for " << magicDamage << " damage\n" << enemy->getStatus() << endl;
                }
            }
            else if (userChoice == 3) {
                cout << hero->listenBard() << endl;
            }
            else if (userChoice == 4) {
                hero->showInventory();
            }
            else if (userChoice == 5) {
                hero->showInventory();
                cout << "Select item to use (enter number): ";
                size_t itemIndex;
                cin >> itemIndex;
                hero->useItemFromInventory(itemIndex - 1);

                if (hero->inventory.getItem(itemIndex - 1) == "Sword") {
                    int originalAttackBonus = hero->attackBonus;
                    hero->attackBonus *= 2;

                    hero->attack(*enemy);
                    cout << hero->getStatus() << endl;
                    hero->attackBonus = originalAttackBonus;
                }
                else if (hero->inventory.getItem(itemIndex - 1) == "Shield") {
                    int originalArmorClass = hero->armorClass;
                    hero->armorClass *= 2;
                    hero->attack(*enemy);
                    cout << hero->getStatus() << endl;
                    hero->armorClass = originalArmorClass;
                }
            }

            else if (userChoice == 6) {
                cout << "The end\n" << hero->getStatus() << "\n" << enemy->getStatus() << endl;
                break;
            }
            else {
                cout << "Wrong choice" << endl;
                continue;
            }

            if (enemy->hitPoints == 0) {
                cout << hero->name << " won" << endl;
                break;
            }

            enemy->attack(*hero);

            if (hero->hitPoints == 0) {
                cout << enemy->name << " won" << endl;
                break;
            }

            cout << "---------------------------------" << endl;
        }

        delete hero;
        delete enemy;
    }
};

int main() {
    setlocale(LC_ALL, "ru");
    Game::startGame();
    return 0;
}
