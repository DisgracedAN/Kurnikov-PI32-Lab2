// Лаб2Структурами.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<locale.h>
#include<conio.h>
#include <Windows.h>
#include<malloc.h>
#include<time.h>
#include<stdlib.h>

//Название классов игрока
static const char* SpecializationNames[] = {
	"Воин",
	"Охотник",
	"Маг"
};


//Классы игрока
enum Specialization {
	Warrior=1,//
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
	Key=1,
	Weapon,
	Armor,
	Consumables
};


//Тип встречающихся NPC
enum EntityType {
	monster=1,
	human,
	animal

};


//Статистика персонажа
struct CharacterClass
{
	char* name;
	Specialization specialization;//Класс
	double HealthBar;//Здоровье
	double Stamina;//Выносливость
	double ManaPoints;//Очки маны
	int SpellCells;//Количество ячеек заклинаний
	double Damage;//Урон

};


//Предметы
struct Item {
	ItemType itemtype;//Тип предмета
	char *Name;//Название
	char *Description;//Краткое описание
	int DropChance;//Шанс выпадения с NPC
};


//NPC
struct Entity {
	char* name;
	EntityType type;//Тип NPC
	int Frendly;//Является ли он дружественным(1-да 0-нет)
	double HealthBar;//Здоровье
	double Stamina;//Выносливость
	double ManaPoints;//Очки маны
	double Damage;//Урон
};


//Инвентарь игрока
struct Inventory
{
	Item items[100];//Массив предметов
	int ActiveSlots;//Свободные слоты
};


//Играбельный персонаж
struct PlayableCharacter
{
	CharacterClass type;
	Inventory inventory;

};


CharacterClass PlayerClassCreate();//Создать класс(Имя и тд)
void PlayerInventoryCreate();//Создать инвентарь
Item ItemCreate(ItemType itemtype, const char* name,const char* desc,int dropchance);//Создать предмет
void AddToInventory(PlayableCharacter* Character, Item item);//Добаваить предмет в инвентарь игрока
void ShowInventory(PlayableCharacter *Character);//Показать инвентарь игрока
void freeInventory(PlayableCharacter* Character);//Функция отчистки 
PlayableCharacter PlayerCharacterSet(CharacterClass type);//Создать игрока(задать класс и инвентарь)
void PlayerOutput(PlayableCharacter character);//Вывод статистики об игроке
Entity EntityCreate(const char* name, EntityType type, int friendly, double health, double stamina, double ManaPoints, double damage);//Создание NPC
Entity AttackEntity(PlayableCharacter *Character, Entity *entity);//Нанесение урона NPC
PlayableCharacter PlayerDamaged(PlayableCharacter* Character, double damage);//Получение урона игроком
int EntityDied(Entity* entity,PlayableCharacter *Character, Item *item);//Смерть NPC и выпадение предмета



int main()
{
	setlocale(LC_ALL, "rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	system("color F0");
	//Создание игрока
	PlayableCharacter Hero;
	CharacterClass player=PlayerClassCreate();
	Hero = PlayerCharacterSet(player);
	//Создание предметов и внесение их в инвентарь
	Item key = ItemCreate(Key, "Башенный ключ", "Старый, потёртый ключ, открывающий дверь в Башню", 0.0);
	AddToInventory(&Hero, key);
	
	Item healthPotion = ItemCreate(Consumables, "Зелье жизни", "Маленьких пузырёк красной жидкости. Единоразово восстанавливает здоровье", 70);
	AddToInventory(&Hero, healthPotion);
	

	//Создание предметов и внесение их в инвентарь в зависимости от класса игрока

	switch (Hero.type.specialization) {
	case(Warrior):
		Item PalladinSword = ItemCreate(Weapon, "Меч Палладина", "Меч, отливающий серебряным блеском. Отлично справляется с нечистью", 40);
		AddToInventory(&Hero, PalladinSword);
		Item PalladinArmor = ItemCreate(Armor, "Кираса Палладина", "Серебристая кираса с гербом Королевста", 45);
		AddToInventory(&Hero, PalladinArmor);
		break;
	case(Hunter):
		Item AssassinDagger = ItemCreate(Weapon, "Кинжал Ассассина", "Клинок из чёрной стали, предназначеный для вероломных ударов в спину", 40);
		AddToInventory(&Hero, AssassinDagger);
		Item HuntersBow = ItemCreate(Weapon, "Лук Охотника", "Простой деревянный лук, ценный среди охотников за простоту и эффективность", 50);
		AddToInventory(&Hero, HuntersBow);
		Item HuntersKilt = ItemCreate(Armor, "Одеяние Охотника", "Лёгкая накидка королевских охотников. Пpостая и эргономичная, не сковывающая движения", 55);
		AddToInventory(&Hero, HuntersKilt);
		break;
	case(Sorcerer):
		Item BasedStaff = ItemCreate(Weapon, "Посох Колдуна", "Обычный посох из дерева и камня души. Позволяет воплощать мысль в магию", 33);
		AddToInventory(&Hero, BasedStaff);
		Item SorcerersRobe = ItemCreate(Armor, "Роба Мага-Новичка", "Роба начинающего мага. Ходят легенты, что хранит в себе частичку магической силы", 55);
		AddToInventory(&Hero, SorcerersRobe);
		break;
	}
	//Вывод статистки
	PlayerOutput(Hero);
	puts("");
	//Просмотр инвентаря
	ShowInventory(&Hero);
	puts("");

	//Создание враждебного NPC 

	Entity Gobbo = EntityCreate("Гоблин-Воин", monster, 0, 50, 2, 3, 5);
	//Создание предмета для выпадения
	Item Money = ItemCreate(Consumables, "Монетка", "Золотая монетка, блестящая на солнце.", 40);
	//Процесс боя

	puts("\nПроцесс боя\n");
	do{
		AttackEntity(&Hero, &Gobbo);
		PlayerDamaged(&Hero, Gobbo.Damage);
	} while (Gobbo.HealthBar != 0);
	puts("\nВыпадение предмета и смерть врага\n");
	EntityDied(&Gobbo,&Hero,&Money);

	puts("\nПросмотр статистики после боя\n");

	PlayerOutput(Hero);

	puts("\nПросмотр инвентаря после боя\n");

	ShowInventory(&Hero);

}



CharacterClass PlayerClassCreate() {
	int PlayerType;
	puts("Введите имя:");
	char* Name = (char*)malloc(30 * sizeof(char));
	fgets(Name, 30, stdin);
	puts("Выберете класс персонажа: 1)Воин 2)Охотник 3)Колдун");
	do {
		scanf("%d", &PlayerType);
	} while (PlayerType < 1 || PlayerType>3);

	CharacterClass playertype;
	playertype.name = Name;
	playertype.specialization = (Specialization)PlayerType;
	switch (PlayerType) {
	case(1):
		playertype.Damage = 20;
		playertype.HealthBar = 100;
		playertype.ManaPoints = 5;
		playertype.Stamina = 50;
		playertype.SpellCells = 0;
		break;
	case(2):
		playertype.Damage = 15;
		playertype.HealthBar = 75;
		playertype.ManaPoints = 20;
		playertype.Stamina = 75;
		playertype.SpellCells = 2;
		break;
	case(3):
		playertype.Damage = 10;
		playertype.HealthBar = 50;
		playertype.ManaPoints = 60;
		playertype.Stamina = 50;
		playertype.SpellCells = 5;
		break;
	default:
		break;
	}
	return playertype;
}


PlayableCharacter PlayerCharacterSet(CharacterClass type) {
	PlayableCharacter Player;
	Player.type = type;
	Player.inventory.ActiveSlots = 0;
	return Player;
}

void PlayerOutput(PlayableCharacter character) {

	printf("Ваше имя: %s\n", character.type.name);
	printf("Ваш класс: %s\n",SpecializationNames[character.type.specialization-1]);
	printf("Статистика:\n Здоровье:%.3lf\n Урон:%.3lf\n Выносливость:%.3lf\n Ячейки заклинаний:%d\n ОМ:%.3lf\n", character.type.HealthBar, character.type.Damage, character.type.Stamina, character.type.SpellCells, character.type.ManaPoints);
}


Item ItemCreate(ItemType itemtype, const char* name,const char* desc,int dropchance) {
	Item newItem;
	newItem.itemtype = itemtype;
	newItem.Name = _strdup(name);
	newItem.Description = _strdup(desc);
	newItem.DropChance = dropchance;
	return newItem;
}

void AddToInventory(PlayableCharacter* Character, Item item) {

	if (Character->inventory.ActiveSlots < 100) {
		Character->inventory.items[Character->inventory.ActiveSlots] = item;
		Character->inventory.ActiveSlots++;
	}
	else {
		printf("Инвентарь полон!\n");
	}

}

void ShowInventory(PlayableCharacter* Character) {
	printf("Содержимое инвентаря:\n");
	for (int i = 0; i < Character->inventory.ActiveSlots; i++) {
		Item item = Character->inventory.items[i];
		printf("Предмет %d: \n", i + 1);
		printf("  Тип: %s\n", ItemNames[item.itemtype-1]);
		printf("  Название: %s\n", item.Name);
		printf("  Описание: %s\n", item.Description);
	}
}

void freeInventory(PlayableCharacter* Character) {
	for (int i = 0; i < Character->inventory.ActiveSlots; i++) {
		free(Character->inventory.items[i].Name);
		free(Character->inventory.items[i].Description);
	}
}

Entity EntityCreate(const char* name, EntityType type, int friendly, double health, double stamina, double ManaPoints, double damage) {
	Entity newEntity;
	newEntity.name = _strdup(name);
	newEntity.type = type;
	newEntity.Frendly = friendly;
	newEntity.HealthBar = health;
	newEntity.Stamina = stamina;
	newEntity.ManaPoints = ManaPoints;
	newEntity.Damage = damage;
	return newEntity;
}

Entity AttackEntity(PlayableCharacter* Character, Entity* entity) {
	if (entity->HealthBar > 0) {
		entity->HealthBar -= Character->type.Damage;
        if (entity->HealthBar < 0) entity->HealthBar = 0;
        printf("%s нанёс %.2f урона по %s. Здоровье %s: %.2f\n",
			Character->type.name, Character->type.Damage, entity->name, entity->name, entity->HealthBar);
    }
	return *entity;
}

PlayableCharacter PlayerDamaged(PlayableCharacter* Character, double damage) {
	if (Character->type.HealthBar > 0) {
		Character->type.HealthBar -= damage;
		if (Character->type.HealthBar < 0) Character->type.HealthBar = 0;
		printf("%s Получил %.2f урона. Здоровье: %.2f\n",
			Character->type.name, damage, Character->type.HealthBar);
	}
	return *Character;
}

int EntityDied(Entity* entity, PlayableCharacter *character, Item *item) {
	srand(time(NULL));
	if (entity->HealthBar <= 0) {
		printf("%s Умер!\n", entity->name);
		// Выпадение предмета
		int dropChance = (0+rand()%100-0+1);
		if (dropChance <= item->DropChance) {
			printf("%s Выронил предмет: %s\n", entity->name, item->Name);
			puts("Подобрать??(1-да 2-нет)");
			int ch;
			do {
				scanf("%d", &ch);
			} while (ch < 1 || ch>2);
			if (ch == 1) {
				AddToInventory(character, *item);
			}
			else return 1;
		}
		else {
			printf("%s НИЧЕГО не выронил\n", entity->name);
			return 1;
		}
	}
	return 0;
}