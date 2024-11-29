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
	string name;
	Specialization specialization;//Класс
	double HealthBar;//Здоровье
	double Stamina;//Выносливость
	double ManaPoints;//Очки маны
	int SpellCells;//Количество ячеек заклинаний
	double Damage;//Урон

	CharacterClass();
	~CharacterClass();
	void setCharacterClass(string pName, Specialization spec,double health, double stamina, double mana, int spellCells, double damage );
	void ClassCreation();
	

};

//Предмет
class Item {
public:
	ItemType itemType;//Тип предмета
	string Name;//Название
	string Description;//Краткое описание
	int DropChance;//Шанс выпадения с NPC

	Item();
	~Item();
	void setItem(ItemType itemtype, string name, string desc, int dropchance);

};

class WeaponItem :public Item {
private:
	double damage;
public:
	WeaponItem();
	~WeaponItem();
	void setWeapon(ItemType itemtype, string name, string desc, int dropchance, double dmg);
	WeaponItem operator++(){
		damage++;
		return *this;
	}
};

class KeyItem :public Item {
private:
	int keyLevel;
public:
	KeyItem();
	~KeyItem();
	void setKey(ItemType itemtype, string name, string desc, int dropchance, int level);

};

class ArmorItem :public Item {
private:
	double defense;
public:
	ArmorItem();
	ArmorItem(ItemType itemtype, string name, string desc, int dropchance, double def);
	~ArmorItem();
	void setArmor(ItemType itemtype, string name, string desc, int dropchance, double def);

	ArmorItem operator + (ArmorItem* Armor2) {
		double resultDefense=this->defense+Armor2->defense;
		return ArmorItem(this->itemType, this->Name, this->Description, this->DropChance, resultDefense);

	}
};

class ConsumableItem :public Item {
private:
	int luckbonus;
	double hpRestor;
public:
	ConsumableItem();
	~ConsumableItem();
	void setConsumable(ItemType itemtype, string name, string desc, int dropchance, int luck, double bonushp);

};



//Инвентарь
class Inventory {
public:
	Item items[100];//Массив предметов
	int ActiveSlots;//Свободные слоты

	Inventory();
	~Inventory();
	bool removeItem(string itemName);
	

};

//Играбельный персонаж
class PlayableCharacter {
	friend void manageInventory(PlayableCharacter& player, Item& item, bool add);
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
	void SetStartItem();
	
};


//NPC
class Entity {
public:
	string Name;
	EntityType Type;//Тип NPC
	bool Friendly;//Является ли он дружественным(1-да 0-нет)
	double HealthBar;//Здоровье
	double Stamina;//Выносливость
	double ManaPoints;//Очки маны
	double Damage;//Урон

	Entity();
	~Entity();
	void setEntity(string name, EntityType type, bool friendly, double health, double stamina, double mana, double damage);
	void printEntity();
	void GetDamaged(PlayableCharacter character);
	void EntityDied(PlayableCharacter &Character, Item item);

};


class PlayableCharacterManager {
private:
	PlayableCharacter* characters;
	static size_t size;
	static size_t capacity;
	void resize();
public:
	PlayableCharacterManager();
	~PlayableCharacterManager();
	void addCharacter(PlayableCharacter& character);
	void removeCharacter(size_t index);
	PlayableCharacter getCharacter(size_t index);
	size_t getSize();
	void update(PlayableCharacter* hero);
	static void printSize();
	static void printCapacity();
	void printFreeSaves();
};


void ShowStatistic(PlayableCharacter& Character, PlayableCharacterManager& saves);

void ShowHeroInventoryTest(PlayableCharacter& Hero);

void CreateEntityAndFight(PlayableCharacter& Hero, Entity& Gobbo);

int main()
{
	setlocale(LC_ALL, "Rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	PlayableCharacterManager saves;
	PlayableCharacter Hero;
	ShowStatistic(Hero, saves);
	
	ShowHeroInventoryTest(Hero);

	//создание врага
	Entity Gobbo;
	CreateEntityAndFight(Hero, Gobbo);

	cout << "Просмотр послебоевой статистики" <<endl ;
	Hero.PrintStats();
	cout << "Просмотр послебоевого инвентаря" << endl;
	Hero.ShowInventory();

	saves.update(&Hero);


	manageInventory(Hero, Hero.inventory.items[1], false);
	Hero.ShowInventory();
	saves.update(&Hero);

	PlayableCharacter hero2;
	hero2.Type.ClassCreation();
	hero2.inventory;
	hero2.SetStartItem();
	saves.addCharacter(hero2);
	
	saves.printSize();
	saves.printCapacity();
	saves.printFreeSaves();

	for (size_t i = 0; i < saves.getSize(); ++i) {
		cout<< saves.getCharacter(i).Type.name<<endl;
	}

	cout << "Введите номер персонажа для вывода статистики:"<<endl;
	saves.printSize();
	int choise;
	do {
		cin >> choise;
		while (getchar() != '\n');
	} while (choise<0||choise>saves.getSize());
	choise--;

	PlayableCharacter character = saves.getCharacter(choise);
	character.PrintStats();
	character.ShowInventory();
	
}

CharacterClass::CharacterClass() {
	
}


void CharacterClass::ClassCreation() {
	int PlayerType;
	puts("Введите имя:");
	string Name;
	getline(cin, Name);
	cout << "Выберете класс персонажа: 1)Воин 2)Охотник 3)Колдун" << endl;
	do {
		cin >> PlayerType;
		while (getchar() != '\n');
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
		stamina = 50;
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
	setCharacterClass(Name, spec, health, stamina, mana, spellCells, damage);
}


void CharacterClass::setCharacterClass(string pName, Specialization spec, double health, double stamina, double mana, int spellCells, double damage) {
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

CharacterClass::~CharacterClass() {

}

Item::Item() {
	DropChance = 0;
}

void Item::setItem(ItemType itemtype, string name, string desc, int dropchance) {
	itemType = itemtype;
	Name = name;
	Description = desc;
	DropChance = dropchance;
}
Item::~Item() {
	
}

Inventory::Inventory() {
	ActiveSlots = 0;
}

Inventory::~Inventory() {
	
}

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

void Entity::setEntity(string name, EntityType type, bool friendly, double health, double stamina, double mana, double damage) {
	Name = name;
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
			cout<<"Подобрать??(1-нет 2-да)"<<endl;
			int ch;
			do {
				cin>>ch;
				while (getchar() != '\n');
			} while (ch < 1 || ch>2);
			ch--;
			bool Ch = (bool)ch;
			manageInventory(Character, item, Ch);
		}
		else {
			cout<< Name<<" НИЧЕГО не выронил"<<endl;
			return ;
		}
	}

}

size_t PlayableCharacterManager::size = 0;
size_t PlayableCharacterManager::capacity = 10;

PlayableCharacterManager::PlayableCharacterManager() {
	
	characters = new PlayableCharacter[capacity];
}

PlayableCharacterManager::~PlayableCharacterManager() {
	delete[] characters;

}

void PlayableCharacterManager::resize() {
	capacity *= 2;
	PlayableCharacter* newCharacters = new PlayableCharacter[capacity];
	for (size_t i = 0; i < size; ++i) {
		newCharacters[i] = characters[i]; // копируем существующие элементы
	}
	delete[] characters;
	characters = newCharacters;
}

void PlayableCharacterManager::addCharacter(PlayableCharacter& character) {

	if (size == capacity) {
		resize();
	}
	characters[size++] = character;
}

void PlayableCharacterManager::removeCharacter(size_t index) {
	if (index < size) {
		// перемещаем последний элемент на место удаляемого
		characters[index] = characters[size - 1];
		size--;
	}
}

PlayableCharacter PlayableCharacterManager::getCharacter(size_t index) {
	if (index < size) {
		return characters[index];
	}
	throw std::out_of_range("Index out of range");
}

size_t PlayableCharacterManager::getSize(){
	return size;
}

void PlayableCharacter::SetStartItem() {
	//создание базовых предметов
	KeyItem *key= new KeyItem();
	key->setKey(Key, "Башенный ключ", "Старый, потёртый ключ, открывающий дверь в Башню", 0.0, 10);
	this->AddToInventory(*key);

	ConsumableItem *healthPotion=new ConsumableItem();
	healthPotion->setConsumable(Consumables, "Зелье жизни", "Маленьких пузырёк красной жидкости. Единоразово восстанавливает здоровье", 70,0,20);
	this->AddToInventory(*healthPotion);

	//распределение вещей в зависимости от "специализации"
	if (this->Type.specialization == Warrior) {
		WeaponItem *PalladinSword=new WeaponItem();
		PalladinSword->setWeapon(Weapon, "Меч Палладина", "Меч, отливающий серебряным блеском. Отлично справляется с нечистью", 40,20);
		ArmorItem *PalladinArmor = new ArmorItem();
		PalladinArmor->setArmor(Armor, "Кираса Палладина", "Серебристая кираса с гербом Королевста", 45,20);
		this->AddToInventory(*PalladinSword);
		this->AddToInventory(*PalladinArmor);
		delete PalladinSword;
		delete PalladinArmor;
	}
	if (this->Type.specialization == Hunter) {
		WeaponItem *AssassinDagger = new WeaponItem();
		AssassinDagger->setWeapon(Weapon, "Кинжал Ассассина", "Клинок из чёрной стали, предназначеный для вероломных ударов в спину", 40, 10);
		WeaponItem *HuntersBow = new WeaponItem();
		HuntersBow->setWeapon(Weapon, "Лук Охотника", "Простой деревянный лук, ценный среди охотников за простоту и эффективность", 50,5);
		ArmorItem *HuntersKilt = new ArmorItem();
		HuntersKilt->setArmor(Armor, "Одеяние Охотника", "Лёгкая накидка королевских охотников. Пpостая и эргономичная, не сковывающая движения", 55,10);
		this->AddToInventory(*AssassinDagger); this->AddToInventory(*HuntersBow); this->AddToInventory(*HuntersKilt);
		delete AssassinDagger; delete HuntersBow; delete HuntersKilt;
	}
	if (this->Type.specialization == Sorcerer) {
		WeaponItem *BasedStuff = new WeaponItem();
		ArmorItem *SorcerersRobe = new ArmorItem();
		BasedStuff->setWeapon(Weapon, "Посох Колдуна", "Обычный посох из дерева и камня души. Позволяет воплощать мысль в магию", 33,20);
		SorcerersRobe->setArmor(Armor, "Роба Мага-Новичка", "Роба начинающего мага. Ходят легенты, что хранит в себе частичку магической силы", 55,2);
		this->AddToInventory(*BasedStuff); this->AddToInventory(*SorcerersRobe);
		delete BasedStuff; delete SorcerersRobe;
	}
	delete key;
	delete healthPotion;
	


}

void PlayableCharacterManager::update(PlayableCharacter* hero) {
	for (size_t i = 0; i < this->getSize(); ++i) {
		if (this->characters[i].Type.name== hero->Type.name) {
			this->characters[i] = *hero;
			return;
		}
	}

}
void PlayableCharacterManager::printSize() {
	cout << "Количество ваших играбельных персонажей: " << size << endl;
}

void PlayableCharacterManager::printCapacity() {
	cout << "Колличество общих ячеек сохранения: " << capacity << endl;
}
void PlayableCharacterManager::printFreeSaves() {
	cout << "Количество свободных ячеек сохранения: " << capacity - size << endl;
}


bool Inventory::removeItem(string itemName) {
	for (int i = 0; i < ActiveSlots; ++i) {
		if (items[i].Name==itemName) {
			// Shift items left
			for (int j = i; j < ActiveSlots - 1; ++j) {
				items[j] = items[j + 1];
			}
			ActiveSlots--;
			return true;
		}
	}
	return false;
}


void manageInventory(PlayableCharacter& player, Item& item, bool add) {
	if (add) {
		player.AddToInventory(item);
	}
	else {
		if (player.inventory.removeItem(item.Name)) {
			cout << "Предмет удалён! "<<endl;
		}
		else {
			cout << "Предмет не найден!" << endl;
		}
	}
}

void ShowStatistic(PlayableCharacter &Character, PlayableCharacterManager &saves) {
	Character.Type.ClassCreation();
	saves.addCharacter(Character);

	cout << "Просмотр статистики после создания" << endl;

	Character.PrintStats();

}

void ShowHeroInventoryTest(PlayableCharacter& Hero) {

	Hero.inventory;

	Hero.SetStartItem();


	cout << "Просмотр инвентаря" << endl;
	Hero.ShowInventory();
}

void CreateEntityAndFight(PlayableCharacter &Hero, Entity &Gobbo) {
	Gobbo.setEntity("Гоблин-Воин", monster, false, 50, 2, 3, 5);
	cout << "Просмотр информации о NPC" << endl;
	Gobbo.printEntity();

	//создание предмета для выпадения
	ConsumableItem* money = new ConsumableItem();
	money->setConsumable(Consumables, "Монетка", "Золотая монетка, блестящая на солнце.", 100,35,0);


	//процесс боя
	do {
		Gobbo.GetDamaged(Hero);
		if (Gobbo.HealthBar <= 0) break;
		Hero.PlayerDamaged(Gobbo.Damage);
		if (Hero.Type.HealthBar <= 0) break;
	} while (Gobbo.HealthBar != 0 || Hero.Type.HealthBar != 0);

	Gobbo.EntityDied(Hero, *money);
	delete money;

}

void KeyItem::setKey(ItemType itemtype, string name, string desc, int dropchance, int level) {
	keyLevel = level;
	setItem(itemtype, name, desc,dropchance);
}

void WeaponItem::setWeapon(ItemType itemtype, string name, string desc, int dropchance, double dmg) {
	damage = dmg;
	setItem(itemtype, name, desc, dropchance);
}
void ArmorItem::setArmor(ItemType itemtype, string name, string desc, int dropchance, double def) {
	defense = def;
	setItem(itemtype, name, desc, dropchance);
}
void ConsumableItem::setConsumable(ItemType itemtype, string name, string desc, int dropchance, int luck, double bonushp) {
	luckbonus = luck;
	hpRestor = bonushp;
	setItem(itemtype, name, desc, dropchance);
}
ArmorItem::ArmorItem() {}

ArmorItem::ArmorItem(ItemType itemtype, string name, string desc, int dropchance, double def)
{
}
ArmorItem::~ArmorItem() {}
WeaponItem::WeaponItem() {}
WeaponItem::~WeaponItem() {}
KeyItem::KeyItem() {}
KeyItem::~KeyItem() {}
ConsumableItem::ConsumableItem() {}
ConsumableItem::~ConsumableItem() {}
