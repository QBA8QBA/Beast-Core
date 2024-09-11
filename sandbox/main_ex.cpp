#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>

/*#include "../includes/Qwindow.h"
#include "../includes/Qsql.h"

// Prosta funkcja sprawdzajaca kolizje AABB (axis align bounding box)
int8 hitbox(int32 x1,int32 y1,int32 width1,int32 height1,int32 x2,int32 y2,int32 width2,int32 height2)
{
    if((x1<x2+width2)&&(x2<x1+width1)&&(y1<y2+height2)&&(y2<y1+height1))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// Glowna funckja rozpoczynajaca wszystko
int main()
{   
    // Definicja kolorow dla Enemy (przedrostek e) i Player (przedrostek p)
    // Kolory sa reprezentowane w rgb
    uint8 eR=0,eG=0,eB=0,pR=0,pG=0,pB=0;
    
    // Petla sprawdzajaca pytajaca o dostep do sieci wmi
    // (bez bycia podloczonym do sieci WMI czy to przez vpn czy prosto z wydzialu
    // niebedzie dalo sie odczytac danych z bazdydanych w my_studio_mycrosoft_sql)
    std::string db="";
    std::cout<<"Czy jestes na sieci wydzialowej : [y/n]\n";
    while(1)
    {
        db.clear();
        std::cin>>db;
        if(db=="Y"){break;}
        if(db=="y"){break;}
        if(db=="N"){break;}
        if(db=="n"){break;}
    }
    std::cout<<"##############\n";
    if((db=="Y") || (db=="y"))
    {
        // Korzystajac z czesci mojej biblioteki Qlib zczytuje dane z tabeli Kolory i je pzrypisuje zmienym 
        // (czesc biblioteki z ktorej korzystam jest zalocznona w folderze lib plik-Qsql.h)
        Table tab;
        ConectToDataBase("mssql-2017.labs.wmi.amu.edu.pl","dbad_s490077","dbad_s490077","KyS41Xsex3");
        if(IsReadable())
        {
            ReadTableFromDataBase(tab,"Kolory");
            uint16 enemyIndex=0;
            uint16 playerIndex=0;
            for(uint16 i=0;i<tab.ColumnVec[0].size();i++)
            {
                if(tab.ColumnVec[0][i]=="Enemy"){enemyIndex=i;}
                if(tab.ColumnVec[0][i]=="Player"){playerIndex=i;}
            }
            eR=(uint8)std::stoi(tab.ColumnVec[1][enemyIndex]);
            eG=(uint8)std::stoi(tab.ColumnVec[2][enemyIndex]);
            eB=(uint8)std::stoi(tab.ColumnVec[3][enemyIndex]);
            pR=(uint8)std::stoi(tab.ColumnVec[1][playerIndex]);
            pG=(uint8)std::stoi(tab.ColumnVec[2][playerIndex]);
            pB=(uint8)std::stoi(tab.ColumnVec[3][playerIndex]);
        }
        else{eR=255;eG=0;eB=255;pR=0;pG=255;pB=128;}
        for(int32 i=0;i<tab.ColumnVec[0].size();i++)
        {
            std::cout<<"Row "<<i<<" --> ";
            for(int32 j=0;j<tab.ColumnVec.size();j++)
            {
                std::cout<<tab.ColumnVec[j][i]<<" | ";
            }
            std::cout<<"\n";
        }
        DisconectFromDataBase();
    }
    else
    {
        eR=255;eG=0;eB=255;pR=0;pG=255;pB=128;
    }

    // Tworzenie obiektow ktore dziedzicza z klasy bazowej Drawable sa przystosowane do reprezentacji prostych krztaltow
    // (Obiekty te sa przystosowane i kompatybilne z Win_32 Api i Win_Gdi Api)
    BackGround bg(0,0,0,255);
    Square player(350,350,10,10,pR,pG,pB,255);
    Square enemy0(0,0,20,40,eR,eG,eB,255);
    Square enemy1(0,0,40,20,eR,eG,eB,255);
    Square enemy2(0,100,20,40,eR,eG,eB,255);
    Square enemy3(100,0,40,20,eR,eG,eB,255);
    Square enemy4(0,200,20,40,eR,eG,eB,255);
    Square enemy5(200,0,40,20,eR,eG,eB,255);
    Square enemy6(0,300,20,40,eR,eG,eB,255);
    Square enemy7(300,0,40,20,eR,eG,eB,255);
    Square enemy8(0,400,20,40,eR,eG,eB,255);
    Square enemy9(400,0,40,20,eR,eG,eB,255);
    std::vector<Square> enemyVec;
    enemyVec.push_back(enemy0);
    enemyVec.push_back(enemy1);
    enemyVec.push_back(enemy2);
    enemyVec.push_back(enemy3);
    enemyVec.push_back(enemy4);
    enemyVec.push_back(enemy5);
    enemyVec.push_back(enemy6);
    enemyVec.push_back(enemy7);
    enemyVec.push_back(enemy8);
    enemyVec.push_back(enemy9);

    // Funkcja tworzaca i obslugujaca nowe okno ktore jest tworzone w osobnym watku
    // (Czesc biblioteki Qlib plik-Qwindow.h)
    SpawnWindow(500,500,"Projekt_Paradygmaty");

    // Wlasciwa petla aplikacji sprawdzajoca czy okno istnieje i dziala
    uint64 fps=0;
    while(IsWindowWorking())
    {
        // Funkcjie przejmujaca prez referencje adresy klasy bazowej
        // (Pozwala to na korzytanie z ich wirtualnej funkcji void Draw(void* buffer,uint32 buffer_width,uint32 buffer_height,uint8 buffer_data_size) bez przeciazania funckji void WindowDrawBuffer(Drawable &obj))
        WindowDrawBuffer(bg);
        
        // Lambda odpowiedzialna za rysowanie obiektow do okna
        std::for_each
        (
            enemyVec.begin(),
            enemyVec.end(),
            [](auto& e)
            {
                WindowDrawBuffer(e);
            }
        );

        // Lambda odpowiedzialna za symulacje ruchu obiektow
        int8 i=0;
        std::for_each
        (
            enemyVec.begin(),
            enemyVec.end(),
            [&i](auto& e)
            {
                if(i%2==0)
                {
                    e.Move(3*(i+1)%11,0);
                }
                else
                {
                    e.Move(0,3*(i+1)%11);
                }
                i++;
            }
        );

        // Lambda odpowiedzialna za sprawdzanie kolizji pomiedzy Player a innymi obiektami
        std::for_each
        (
            enemyVec.begin(),
            enemyVec.end(),
            [&player](auto& e)
            {
                if(hitbox(player.x,player.y,player.width,player.height,e.x,e.y,e.width,e.height))
                {
                    DespawnWindow();
                }
            }
        );
        WindowDrawBuffer(player);
        
        // Prezentacja buffera 
        // (Qlib dziala obecnie w trybie 2 bufferow jeden jest przygotowywany a drugi rysowany i sie zamieniaja kiedy oba skoncza prace)

        WindowSwapBuffer();
        // Szybki if na zatrzymanie gry zaraz po wpisaniu "No" lub "Yes"
        if(fps==0){std::this_thread::sleep_for(std::chrono::milliseconds(1000));}
        
        // Zczytywanie informacji o klawiszach
        // Czesc biblioteki Qlib zaimplemetowalem na szybko tylko klawisze W,A,S,D
        if(IsKeyPresed('W')){player.Move(0,-5);}
        if(IsKeyPresed('A')){player.Move(-5,0);}
        if(IsKeyPresed('S')){player.Move(0,5);}
        if(IsKeyPresed('D')){player.Move(5,0);}
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        fps++;
    }
    // Funckja odpowiedzialna za upewnienie sie ze watek wyczysci cala pamiec ktora zarezerwowal oraz ze zostanie przerwany i sie zakonczy
    DespawnWindow();


    //Wyswietlenie GameOver i zamkniecie okna
    std::cout<<"GAME OVER\n";
    std::cin.get();
}
*/