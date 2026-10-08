#include <iostream> 
#include <vector>
#include <memory>
using namespace std;
//ключевые понятия (принципы) ООП:
//1) наследование
//2) полиморфизм
//3) абстракция
//4) инкапсуляция (геттеры и сеттеры)

// модификаторы доступа:
// public - публичный (доступен внутри класса, внутри наследника и в основном потоке программы)
// private - приватный (доступен только в исходном классе)
// private - НЕ НАСЛЕДУЕТСЯ 
// protected - защищенный (можно изменять в исходном классе и в классах наследниках)

// Если хотя бы один метод/поле виртуальное, то это абстрактный класс который вызвать нельзя
class NPC
{
protected:
    string name{ "npc" };
    unsigned int damage{ 2 };
    unsigned int health{ 5 };
    short lvl = 1;
    unsigned int armor = 2;
private:
    bool isEnemy = true;
public:
    // Создание конструктора в учебных целях
    // NPC()
    // {   
    //     cout << "NPC создан" << "\n";
    //     name = "npc";
    //     damage = 2;
    //     health = 5;
    //     lvl = 1;
    //     armor = 2;
    // }

    // сеттеры
    unsigned int GetDamage() { return damage; }
    string GetName() { return name; }
    unsigned int GetHealth() { return health; }
    short int GetLvl() { return lvl; }
    unsigned int GetArmor() { return armor; }

    void SetHealth(unsigned int health) { this->health = health; } // геттер

    virtual void GetInfo()
    {
        cout << "имя: " << name << endl;
        cout << "здоровье: " << health << endl;
        cout << "урон: " << damage << endl;
        cout << "уровень: " << lvl << endl;
        cout << "броня: " << armor << endl;
    };
    // Создать NPC нельзя, значит весь класс виртуальный
    virtual void Create() { isEnemy = false; };
    void LvlUp()
    {
        cout << name << " получил новый уровень" << endl;
        lvl;
        Relaculate();
    }
    void Relaculate() //кастомизировать для война и волшебника зависимости от инт/силы
    {
        damage += (1 + lvl * 0.1);
        health += (1 + lvl * 0.1);
        armor += (1 + lvl * 0.1);
    }

    // Создание деструктора в учебных целях
    // ~NPC()
    // {
    //     cout << "NPC уничтожен" << "\n";
    // }
    virtual ~NPC() = default;
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
    //конструктор по умолчанию
    Warrior()
    {
        //cout << "конструктор война" << endl;
        damage = 20;
        health = 30;
        armor = 15;
    }
    //кастомный конструктор
    Warrior(string name, unsigned int lvl)
    {
        damage = 20;
        health = 30;
        armor = 15;
        this->name = name; //такой способ задания поля уместен только для сеттер
        for (size_t i = 0; i < lvl; ++i)
        {
            LvlUp();
        }
        //this->lvl = lvl; //this - указывает на конкретный экземпляр класса
    }
    void Create() override
    {
        cout << "Вы создали война\nЗадайте имя игрока\n";
        cin >> name;

        GetInfo();
        GetWeapon();
    };
    void GetInfo() override
    {
        NPC::GetInfo();
        cout << "Персонаж: воин\n";
        cout << "сила: " << strength << endl;
    };
    void GetWeapon()
    {
        Weapon weapon;
        weapon.damage = 1;
        weapon.name = "кулаки";
        weapons.push_back(weapon);

        cout << name << " взял в руки оружие " << weapons[0].name << endl;
        cout << "добавка к урону = " << weapons[0].damage << endl;
    };
    ~Warrior() //деструктор (вызывается сам в момент высвобождения памяти экземпляра)
    {
        cout << name << " пал смертью храбрых" << endl;
    }
};

struct Spell//придумать интересный деструктор для злодеев
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
    Wizard()//конструктор вызывается в момент создания экземпляра
    {
        //cout << "конструктор волшебник" << endl;
        damage = 27;
        health = 21;
        armor = 10;
    }
    void Create() override
    {
        cout << "Вы создали волшебник\nЗадайте имя игрока\n";
        cin >> name;

        GetInfo();
        LearnSpell();
    };
    void GetInfo() override
    {
        NPC::GetInfo();
        cout << "Персонаж: маг\n";
        cout << "интеллект: " << intellect << endl;
    };
    void LearnSpell()
    {
        Spell spell;
        spell.damage = 2;
        spell.name = "вспышка";
        spells.push_back(spell);

        cout << name << " изучил заклинание " << spells[0].name << endl;
            cout << "добавка к урону = " << spells[0].damage << endl;
    };
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
    Evil(string name) : Evil() //делегирование конструктора (то есть вызовется вначале базовый)
    {
        this->name = name;
    }
    Evil(string name, unsigned int damage) : Evil(name)
    {
        this->damage = damage;
    }
    Evil(string name, unsigned int damage, unsigned int health) : Evil(name, damage)
    {
        this->health = health;
    }
    Evil(string name, unsigned int damage, unsigned int health, unsigned int armor) : Evil(name, damage, health)
    {
        this->armor = armor;
    }
    //придумать интересный деструктор для злодеев
};

// множественное наследование!!!!
class Paladin : public Warrior, public Wizard // сколько угодно
{
public:
    Paladin()
    {
        intellect = 25;
        strength = 19;
        health = 25; // неоднозначность, и в том и в том классе есть это поле
        damage = 25; // неоднозначность, и в том и в том классе есть это поле (фиксится за счет virtual)
    }

    void Create() override
    {
        // Реализуйте это в NPC (храните названия класса в каком то поле)
        cout << "Вы создали паладина\nЗадайте имя игрока\n";
        cin >> name;

        GetInfo();
        GetWeapon();
        LearnSpell();
    };
    void GetInfo() override
    {
        NPC::GetInfo();
        cout << "интеллект: " << intellect << endl;
    };
    ~Paladin()
    {
        cout << name << " отправляется к праотцам" << "\n";
    }
};

// дружественные классы 
class Player
{
private:
    unique_ptr<NPC> currentCharacter{ nullptr };
public:
    void Create(unique_ptr<NPC> character)
    {
        currentCharacter = move(character);
        currentCharacter->Create();
    }
    NPC* GetCharacter()
    {
        return currentCharacter.get();
    }
    void GetInfo()
    {
        if (currentCharacter) {
            currentCharacter->GetInfo();
        }
        else cout << "Персонаж не создан\n";
    }
};

int main()
{
    Player player;

    cout << "Присядь путник у костра и расскажи, кто ты" << "\n";
    cout << "\t1 - воин\n\t2 - волшебник\n\t3 - паладин" << "\n";
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
        cout << "Таких героев еще не было в наших краях...\n";
    }
    player.GetInfo();
    // Warrior* warrior = new Warrior("друг война", 5); //как только создал новый экземпляр, для него вызвался конструктор
    // warrior->GetInfo(); //стрелка работает с указателями
       // warrior->GetHealth();
    // delete warrior;
    // warrior = nullptr;

    // Wizard wizard;

    // // превратить злодеев в вектор (указателей, умных)
    // Evil evil1, evil2("Кабанчик"), evil3("Гнолл", 12),
    //  evil4("Гнолл Дробитель",15,20), evil5("Дракон",50,100,200);

    // evil1.GetInfo();
    // evil2.GetInfo();
    // evil3.GetInfo();
    // evil4.GetInfo();
    // evil5.GetInfo();

    return 0;
}