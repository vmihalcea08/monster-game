#include <iostream>
#include <string>

class Creature
{
protected:
    std::string m_name {};
    char m_symbol {};
    int m_health {};
    int m_damage {};
    int m_gold {};

public:
    Creature(std::string_view name, char symbol, int health, int damage, int gold)
    : m_name { name }
    , m_symbol { symbol }
    , m_health { health }
    , m_damage { damage }
    , m_gold { gold }
    {}
    
    //getters
    const std::string& getName() const { return m_name; }
    char getSymbol() const { return m_symbol; }
    int getHealth() const { return m_health; }
    int getDamage() const { return m_damage; }
    int getGold() const { return m_gold; }
    
    void reduceHealth (int reduce) { m_health = m_health - reduce; }
    
    bool isDead() const
    {
        if (m_health <= 0)
            return true;
        else 
            return false;
    }
    
    void addGold (int increase) { m_gold = m_gold + increase; }
};

class Player : public Creature
{
private:
    int m_level { 1 };

public:
    Player (std::string_view name)
    : Creature {name, '@', 10, 1, 0 }
    {}
    
    void levelUp() 
    {
        ++ m_level;
        ++ m_damage;
    }
    
    int getLevel() const { return m_level; }
    
    bool hasWon() const
    {
        if (m_level == 20)
            return true;
        else
            return false;
    }
};

class Monster : public Creature
{
public:
    enum Type
    {
        dragon,
        orc,
        slime,
        max_types,
    };

private:
    static inline Creature monsterData[] 
    {
        {"dragon", 'D', 20, 4, 100},
        {"orc", 'o', 4, 2, 25},
        {"slime", 's', 1, 1, 10}
    }; 
    
    static_assert(std::size(monsterData) == max_types);

public:
    Monster (Type type)
    : Creature { monsterData[type] }
    {}
    
    static Monster getRandomMonster()
    {
        int num { Type::slime };
        return Monster {static_cast<Type>(num)};
    }
};

void attackPlayer(Player& player, const Monster& monster)
{
    if (monster.isDead())
        return;
    
    player.reduceHealth(monster.getDamage());
    std::cout << "The " << monster.getName() << " hit you for " << monster.getDamage() << " damage.\n";
}

void attackMonster(Monster& monster, Player& player)
{
    if (player.isDead())
        return;
        
    std::cout << "You hit the " << monster.getName() << " for " << player.getDamage() << " damage\n";
        
    monster.reduceHealth(player.getDamage());
        
    if (monster.isDead())
    {
        std::cout << "You killed the " << monster.getName() << ".\n";
        player.levelUp();
        std::cout << "You are now level " << player.getLevel() << ".\n";
        std::cout << "You found " << monster.getGold() << " gold.\n";
        player.addGold(monster.getGold());
    }
}

void fightMonster(Player& player)
{
    Monster monster { Monster::getRandomMonster() };
    std::cout << "You have encountered a " << monster.getName() << " (" << monster.getSymbol() << ").\n";
    
    while (!monster.isDead() && !player.isDead())
    {
        std::cout << "(R)un or (F)ight: ";
        char input{};
        std::cin >> input;
        if (input == 'R' || input == 'r')
        {
            attackPlayer(player, monster);
            continue;
        }
        else
            attackMonster(monster, player);
    }
}


int main() 
{
    std::cout << "Enter your name: ";
    
    std::string name;
    std::getline(std::cin, name);
    
    std::cout << "Hello, " << name << ".\n";
    
    Player player {name};
    
    std::cout << "You have " << player.getHealth() << " and are carrying " << player.getGold() << " gold.\n\n";
    
    
    Monster monster { Monster::Type::slime };
	std::cout << "A " << monster.getName() << " (" << monster.getSymbol() << ") was created.\n";
	
	while (!player.isDead() && !player.hasWon())
        fightMonster(player);

    
    if (player.isDead())
    {
        std::cout << "You died at level " << player.getLevel() << " and with " << player.getGold() << " gold.\n";
        std::cout << "Too bad you can't take it with you!\n";
    }
    else
    {
        std::cout << "You won the game with " << player.getGold() << " gold!\n";
    }
    
    return 0;
}
