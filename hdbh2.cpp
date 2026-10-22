#include <iostream>
#include <vector>
#include <memory>
using namespace std;


class NPC
{
public:
    string name{ "npc" };
    unsigned int damage{ 2 };
    unsigned int health{ 5 };
    short lvl = 1;
    unsigned int armor = 2;
private:
    bool isEnemy = true;
public:
    unsigned int GetDamage() { return damage; }
    unsigned int GetHealth() { return health; }        // геттер
    void SetHealth(unsigned int health) { this->health = health; } // сеттер

    virtual void GetInfo()
    {
        cout << "имя: " << name << endl;
        cout << "здоровье: " << health << endl;
        cout << "урон: " << damage << endl;
        cout << "уровень: " << lvl << endl;
        cout << "броня: " << armor << endl;
    }

    // Создать NPC нельзя, поэтому он виртуальный
    virtual void Create() {}

    void LvlUp()
    {
        cout << name << " получил новый уровень" << endl;
        lvl++;              // было: lvl; — ничего не делало
        Relaculate();
    }

    void Relaculate()
    {
        damage += (1 + lvl * 0.1);
        health += (1 + lvl * 0.1);
        armor += (1 + lvl * 0.1);
    }

    virtual ~NPC() {}
};

struct Weapon
{
    string name{ "weapon" };
    unsigned int damage{ 1 };
};

class Warrior : virtual public NPC
{
protected:
    short strength{ 21 };
    vector<Weapon> weapons;
public:
    Warrior()
    {
        damage = 20;
        health = 30;
        armor = 15;
        Create();
    }

    Warrior(string name, unsigned int lvl)
    {
        damage = 20;
        health = 30;
        armor = 15;
        this->name = name;
        for (size_t i = 0; i < lvl; i++)
        {
            LvlUp();
        }
    }

    void Create() override
    {
        cout << "Вы создали воина\nЗадайте имя игрока\n";
        cin >> name;

        GetInfo();
        GetWeapon();
    }

    void GetInfo() override
    {
        NPC::GetInfo();
        cout << "сила: " << strength << endl;
    }

    void GetWeapon()
    {
        Weapon weapon;
        weapon.damage = 1;
        weapon.name = "кулаки";
        weapons.push_back(weapon);

        cout << name << " взял в руки оружие " << weapons[0].name << endl;
        cout << " добавка к урону = " << weapons[0].damage << endl;
    }

    ~Warrior()
    {
        cout << name << " пал смертью храбрых" << endl;
    }
};

struct Spell
{
    string name{ "spell" };
    unsigned int damage{ 1 };
};

class Wizard : virtual public NPC
{
protected:
    short intellect{ 29 };
    vector<Spell> spells;
public:
    Wizard()
    {
        damage = 27;
        health = 21;
        armor = 10;
        Create();
    }

    void Create() override
    {
        cout << "Вы создали волшебника\nЗадайте имя игрока\n";
        cin >> name;

        GetInfo();
        LearnSpell();
    }

    void GetInfo() override
    {
        NPC::GetInfo();
        cout << "интеллект: " << intellect << endl;
    }

    void LearnSpell()
    {
        Spell spell;
        spell.damage = 2;
        spell.name = "вспышка";
        spells.push_back(spell);

        cout << name << " изучил заклинание " << spells[0].name << endl;
        cout << " добавка к урону = " << spells[0].damage << endl;
    }

    ~Wizard()
    {
        cout << name << " испускает дух" << endl;
    }
};

class Evil : public NPC
{
public:
    Evil()
    {
        name = "Злодей";
        health = 10;
        damage = 5;
        armor = 3;
    }
    Evil(string name) : Evil() { this->name = name; }
    Evil(string name, unsigned int damage) : Evil(name) { this->damage = damage; }
    Evil(string name, unsigned int damage, unsigned int health) : Evil(name, damage) { this->health = health; }
    Evil(string name, unsigned int damage, unsigned int health, unsigned int armor) : Evil(name, damage, health) { this->armor = armor; }

    ~Evil()
    {
        cout << name << " был повержен героем!" << endl;
    }
};

class Paladin : public Warrior, public Wizard
{
public:
    Paladin()
    {
        intellect = 25;
        strength = 25;
        health = 25;
        damage = 25;
    }

    void Create() override
    {
        cout << "Вы создали паладина\nЗадайте имя игрока\n";
        cin >> name;

        GetInfo();
        GetWeapon();
        LearnSpell();
    }

    void GetInfo() override
    {
        NPC::GetInfo();
        cout << "сила: " << strength << endl;
        cout << "интеллект: " << intellect << endl;
    }

    ~Paladin()
    {
        cout << name << " отправляется к праотцам" << endl;
    }
};

class Player
{
private:
    unique_ptr<NPC> currentCharacter{ nullptr };
public:
    void Create(unique_ptr<NPC> character)
    {
        currentCharacter = move(character);
    }

    NPC* GetCharacter()
    {
        return currentCharacter.get();
    }
};

void TakeDamage(NPC* npc, unsigned int damage)
{
    npc->SetHealth(npc->health - damage);
    cout << "Вам нанесли урон: " << damage << endl;;
    cout << "Оставшееся здоровье = " << npc->health << endl;;
}

int main()
{
    setlocale(LC_ALL, "RUS");

    Player player;

    cout << "Присядь, путник, у костра и расскажи, кто ты: " << endl;
    cout << "\t1 - Воин\n\t2 - Волшебник\n\t3 - Паладин" << endl;

    short choice = 0;
    cin >> choice;

    switch (choice)
    {
    case 1:
        player.Create(make_unique<Warrior>());
        break;
    case 2:
        player.Create(make_unique<Wizard>());
        break;
    case 3:
        player.Create(make_unique<Paladin>());
        break;
    default:
        cout << "Таких героев ещё не было в наших краях.\nПопытай удачу позже" << endl;
    }
    Warrior* warrior = new Warrior("друг воина", 5);
    warrior->GetInfo();
    cout << "Метод публичный, вот значение здоровья: " << warrior->GetHealth() << endl;
    delete warrior;
    warrior = nullptr;

    vector<unique_ptr<Evil>> evils;
    evils.push_back(make_unique<Evil>());
    evils.push_back(make_unique<Evil>("Кабанчик"));
    evils.push_back(make_unique<Evil>("Гнолл", 12));
    evils.push_back(make_unique<Evil>("Гнолл Дробитель", 15, 20));
    evils.push_back(make_unique<Evil>("Дракон", 50, 100, 200));

    for (auto& e : evils)
        e->GetInfo();

    return 0;
}
