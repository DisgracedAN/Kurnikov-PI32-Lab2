// Lab2classes.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <ctime>
#include<string>
#include<cstdlib>
#include<windows.h>
using namespace std;

//Название классов игрока
static const char* SpecializationNames[] = {
	"Воин",
	"Охотник",
	"Маг"
};


//Классы игрока
enum Specialization {
	Warrior = 1,//
	Hunter,//
	Sorcerer//
};


//Названия типов предметов
static const char* ItemNames[] = {
	"Ключ",
	"Оружие",
	"Броня",
	"Расходуемые предметы"
};


//Типы предметов
enum ItemType {
	Key = 1,
	Weapon,
	Armor,
	Consumables
};


//Названия типов NPC
static const char* EntityNames[] = {
	"Монстр",
	"Человек",
	"Животное"
	
};

//Тип встречающихся NPC
enum EntityType {
	monster = 1,
	human,
	animal

};

//Класс персонажа
class CharacterClass {
public:
	char* name;
	Specialization specialization;//Класс
	double HealthBar;//Здоровье
	double Stamina;//Выносливость
	double ManaPoints;//Очки маны
	int SpellCells;//Количество ячеек заклинаний
	double Damage;//Урон

	CharacterClass();
	~CharacterClass();
	void setCharacterClass(char* pName, Specialization spec,double health, double stamina, double mana, int spellCells, double damage );
	


};

//Предмет
class Item {
public:
	ItemType itemType;//Тип предмета
	char* Name;//Название
	char* Description;//Краткое описание
	int DropChance;//Шанс выпадения с NPC

	Item();
	~Item();
	void setItem(ItemType itemtype, const char* name, const char* desc, int dropchance);

};


//Инвентарь
class Inventory {
public:
	Item items[100];//Массив предметов
	int ActiveSlots;//Свободные слоты

	Inventory();
	~Inventory();
	

};

//Играбельный персонаж
class PlayableCharacter {
public:
	CharacterClass Type;
	Inventory inventory;
public:
	PlayableCharacter();
	~PlayableCharacter();
	void PrintStats();
	void ShowInventory();
	void AddToInventory(Item item);
	void PlayerDamaged(double damage);
};


//NPC
class Entity {
public:
	char* Name;
	EntityType Type;//Тип NPC
	bool Friendly;//Является ли он дружественным(1-да 0-нет)
	double HealthBar;//Здоровье
	double Stamina;//Выносливость
	double ManaPoints;//Очки маны
	double Damage;//Урон

	Entity();
	~Entity();
	void setEntity(const char* name, EntityType type, bool friendly, double health, double stamina, double mana, double damage);
	void printEntity();
	void GetDamaged(PlayableCharacter character);
	void EntityDied(PlayableCharacter &Character, Item item);

};

int main()
{
	setlocale(LC_ALL, "Rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	PlayableCharacter Hero;

	cout << "Просмотр статистики после создания"<<endl;
	
	Hero.PrintStats();
	Hero.inventory;

	//создание базовых предметов
	Item key;
	key.setItem(Key, "Башенный ключ", "Старый, потёртый ключ, открывающий дверь в Башню", 0.0);
	Hero.AddToInventory(key);

	Item healthPotion;
	healthPotion.setItem(Consumables, "Зелье жизни", "Маленьких пузырёк красной жидкости. Единоразово восстанавливает здоровье", 70);
	Hero.AddToInventory(healthPotion);

	//распределение вещей в зависимости от "специализации"
	if (Hero.Type.specialization == Warrior) {
		Item PalladinSword;
		PalladinSword.setItem(Weapon, "Меч Палладина", "Меч, отливающий серебряным блеском. Отлично справляется с нечистью", 40);
		Item PalladinArmor;
		PalladinArmor.setItem(Armor, "Кираса Палладина", "Серебристая кираса с гербом Королевста", 45);
		Hero.AddToInventory(PalladinSword);
		Hero.AddToInventory(PalladinArmor);
	}
	if (Hero.Type.specialization == Hunter) {
		Item AssassinDagger;
		AssassinDagger.setItem(Weapon, "Кинжал Ассассина", "Клинок из чёрной стали, предназначеный для вероломных ударов в спину", 40);
		Item HuntersBow;
		HuntersBow.setItem(Weapon, "Лук Охотника", "Простой деревянный лук, ценный среди охотников за простоту и эффективность", 50);
		Item HuntersKilt;
		HuntersKilt.setItem(Armor, "Одеяние Охотника", "Лёгкая накидка королевских охотников. Пpостая и эргономичная, не сковывающая движения", 55);
		Hero.AddToInventory(AssassinDagger); Hero.AddToInventory(HuntersBow); Hero.AddToInventory(HuntersKilt);
	}
	if (Hero.Type.specialization == Sorcerer) {
		Item BasedStuff;
		Item SorcerersRobe;
		BasedStuff.setItem(Weapon, "Посох Колдуна", "Обычный посох из дерева и камня души. Позволяет воплощать мысль в магию", 33);
		SorcerersRobe.setItem(Armor, "Роба Мага-Новичка", "Роба начинающего мага. Ходят легенты, что хранит в себе частичку магической силы", 55);
		Hero.AddToInventory(BasedStuff); Hero.AddToInventory(SorcerersRobe);
	}

	
	cout << "Просмотр инвентаря" <<endl ;
	Hero.ShowInventory();

	//создание врага
	Entity Gobbo;
	Gobbo.setEntity("Гоблин-Воин", monster, false, 50, 2, 3, 5);
	cout << "Просмотр информации о NPC" <<endl ;
	Gobbo.printEntity();

	//создание предмета для выпадения
	Item money;
	money.setItem(Consumables, "Монетка", "Золотая монетка, блестящая на солнце.", 40);


	//процесс боя
	do {
		Gobbo.GetDamaged(Hero);
		if (Gobbo.HealthBar <= 0) break;
		Hero.PlayerDamaged(Gobbo.Damage);
		if (Hero.Type.HealthBar <= 0) break;
	} while (Gobbo.HealthBar != 0||Hero.Type.HealthBar!=0);

	Gobbo.EntityDied(Hero, money);

	cout << "Просмотр послебоевой статистики" <<endl ;
	Hero.PrintStats();
	cout << "Просмотр послебоевого инвентаря" << endl;
	Hero.ShowInventory();
	
}

CharacterClass::CharacterClass() {
	int PlayerType;
	puts("Введите имя:");
	char* Name = (char*)malloc(30 * sizeof(char));
	fgets(Name, 30, stdin);
	puts("Выберете класс персонажа: 1)Воин 2)Охотник 3)Колдун");
	do {
		cin>>PlayerType;
	} while (PlayerType < 1 || PlayerType>3);
	Specialization spec = (Specialization)PlayerType;
	double health;
	double stamina;
	double mana;
	int spellCells;
	double damage;

	switch (PlayerType) {
	case(1):
		damage = 20;
		health = 100;
		mana = 5;
		stamina= 50;
		spellCells = 0;
		break;
	case(2):
		damage = 15;
		health = 75;
		mana = 20;
		stamina = 75;
		spellCells = 2;
		break;
	case(3):
		damage = 10;
		health = 50;
		mana = 60;
		stamina = 50;
		spellCells = 5;
		break;
	default:
		break;
	}
	setCharacterClass(Name,spec,health, stamina,mana,spellCells,damage);
}

void CharacterClass::setCharacterClass(char* pName, Specialization spec, double health, double stamina, double mana, int spellCells, double damage) {
	name = pName;
	specialization=spec;
    HealthBar=health;
	Stamina=stamina;
	ManaPoints=mana;
	SpellCells=spellCells;
	Damage=damage;
}

void PlayableCharacter::PrintStats() {
	cout << "Ваше имя: " << Type.name << endl;
	cout << "Ваш Класс: " << SpecializationNames[Type.specialization - 1] << endl;
	cout << "Ваша статистика:\nЗдоровье: "<< Type.HealthBar <<endl;
	cout << "Урон: " << Type.Damage << endl;
	cout << "Выносливость: " << Type.Stamina << endl;
	cout << "Ячейки заклинаний: " << Type.SpellCells << endl;
	cout << "ОМ: " << Type.ManaPoints << endl;
}

CharacterClass::~CharacterClass() {}

Item::Item() {
	DropChance = 0;
}

void Item::setItem(ItemType itemtype, const char* name, const char* desc, int dropchance) {
	itemType = itemtype;
	Name = _strdup(name);
	Description = _strdup(desc);
	DropChance = dropchance;
}
Item::~Item() {}

Inventory::Inventory() {
	ActiveSlots = 0;
}

Inventory::~Inventory() {}

void PlayableCharacter::AddToInventory(Item item) {
	if (inventory.ActiveSlots < 100) {
		inventory.items[inventory.ActiveSlots] = item;
		inventory.ActiveSlots++;
	}
	else {
		printf("Инвентарь полон!\n");
	}
}

void PlayableCharacter::ShowInventory() {
	cout<<"Содержимое инвентаря"<<endl;
	for (int i = 0; i < inventory.ActiveSlots; i++) {
		Item item = inventory.items[i];
		cout << "Предмет: " << i + 1 << endl;
		cout<<"  Тип: "<< ItemNames[item.itemType - 1]<<endl;
		cout<<"  Название: "<< item.Name<<endl;
		cout << "  Описание: "<< item.Description<<endl;
	}
}

PlayableCharacter::PlayableCharacter() {}

PlayableCharacter::~PlayableCharacter() {}

Entity::Entity() {}


Entity::~Entity() {}

void Entity::setEntity(const char* name, EntityType type, bool friendly, double health, double stamina, double mana, double damage) {
	Name = _strdup(name);
	Type = type;
	Friendly = friendly;
	HealthBar = health;
	Stamina = stamina;
	ManaPoints = mana;
	Damage = damage;
}

void Entity::printEntity() {
	cout << "Имя NPC: " << Name << endl;
	cout << "Тип NPC: " << EntityNames[Type - 1] << endl;
	cout << "Cтатистика:\nЗдоровье: " << HealthBar << endl;
	cout << "Урон: " << Damage << endl;
	cout << "Выносливость: " << Stamina << endl;
	cout << "ОМ: " << ManaPoints << endl;
}

void Entity::GetDamaged(PlayableCharacter character) {
	if (HealthBar > 0) {
		HealthBar -= character.Type.Damage;
		if (HealthBar < 0) HealthBar = 0;
		cout<<character.Type.name<<" нанёс "<< character.Type.Damage<<" урона по "<< Name<<" Здоровье:  "<< Name<<" составляет "<< HealthBar << endl;
	}
}

void PlayableCharacter::PlayerDamaged(double damage) {
	if (Type.HealthBar > 0) {
		Type.HealthBar -= damage;
		if (Type.HealthBar < 0) Type.HealthBar = 0;
		cout<<Type.name<<"Получил "<< damage<<" урона. Текущее здоровье:  "<< Type.HealthBar<<endl;
	}

}

void Entity::EntityDied(PlayableCharacter &Character, Item item) {
	srand(time(NULL));
	if (HealthBar <= 0) {
		cout<<Name<<" погиб!"<<endl;
		// Выпадение предмета
		int dropChance = (0 + rand() % 100 - 0 + 1);
		if (dropChance <= item.DropChance) {
			cout<< Name<<" Выронил: "<< item.Name<<endl;
			cout<<"Подобрать??(1-да 2-нет)"<<endl;
			int ch;
			do {
				cin>>ch;
			} while (ch < 1 || ch>2);
			if (ch == 1) {
				Character.AddToInventory(item);
			}
			else return ;
		}
		else {
			cout<< Name<<"НИЧЕГО не выронил"<<endl;
			return ;
		}
	}

}