#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct Artist {
    string name;
    string genre;
    int yearBorn;
};

class Song;   // Forward declaration
class Album;  // Forward declaration

// ===== FRIEND FUNCTIONS =====
void displaySong(const Song& s);
void displayAlbum(const Album& a);

class Media {
protected:
    string title;

public:
    Media(string t = "Unknown") : title(t) {}
    string getTitle() const { return title; }
    void setTitle(const string& t) { title = t; }
};

class Song : public Media {
private:
    Artist artist;
    double duration;

public:
    Song() : Media("Untitled"), duration(0.0) {}

    Song(string t, Artist a, double d)
        : Media(t), artist(a), duration(d) {}

    void display() const {
        displaySong(*this);   // friend function
    }

    // getters
    Artist getArtist() const { return artist; }
    double getDuration() const { return duration; }

    // for editing
    void setSong(string t, Artist a, double d) {
        title = t;
        artist = a;
        duration = d;
    }

    // Declare friend
    friend void displaySong(const Song& s);
};

class Album : public Media {
private:
    Song songs[10];
    int numSongs;

public:
    Album(string t = "Untitled") : Media(t), numSongs(0) {}

    void addSong(const Song& s) {
        if (numSongs < 10) {
            songs[numSongs++] = s;
            cout << "Song added.\n";
        } else {
            cout << "Album is full.\n";
        }
    }

    void deleteSong(int index) {
        if (index >= 0 && index < numSongs) {
            for (int i = index; i < numSongs - 1; i++)
                songs[i] = songs[i + 1];
            numSongs--;
            cout << "Song deleted.\n";
        } else {
            cout << "Invalid song index.\n";
        }
    }

    void editSong(int index) {
        if (index >= 0 && index < numSongs) {

            char t[100], name[100], gen[100];
            int y;
            double d;

            cin.ignore();
            cout << "New song title: ";
            cin.getline(t, 100);

            cout << "Artist name: ";
            cin.getline(name, 100);

            cout << "Genre: ";
            cin.getline(gen, 100);

            cout << "Year Born: ";
            cin >> y;

            cout << "Duration: ";
            cin >> d;

            Artist a = {name, gen, y};
            songs[index].setSong(t, a, d);

            cout << "Song updated.\n";
        } else {
            cout << "Invalid index.\n";
        }
    }

    void display() const {
        displayAlbum(*this);
    }

    int getNumSongs() const { return numSongs; }
    Song getSong(int i) const { return songs[i]; }

    // Declare friend
    friend void displayAlbum(const Album& a);
};

// ===== FRIEND FUNCTION DEFINITIONS =====

void displaySong(const Song& s) {
    cout << "Song: " << s.title << endl;
    cout << "Artist: " << s.artist.name
         << " | Genre: " << s.artist.genre
         << " | Born: " << s.artist.yearBorn << endl;
    cout << "Duration: " << s.duration << " mins\n";
}

void displayAlbum(const Album& a) {
    cout << "\nAlbum: " << a.title << endl;
    cout << "Songs: " << a.numSongs << endl;
    for (int i = 0; i < a.numSongs; i++) {
        cout << "---- Song #" << i << " ----\n";
        a.songs[i].display();
    }
}

// =================== MUSIC LIBRARY ===================

class MusicLibrary {
private:
    Album albums[5];
    int numAlbums;

public:
    MusicLibrary() : numAlbums(0) {}

    void addAlbum(string title) {
        if (numAlbums < 5) {
            albums[numAlbums++] = Album(title);
            cout << "Album added.\n";
        } else {
            cout << "Library full.\n";
        }
    }

    void deleteAlbum(int index) {
        if (index >= 0 && index < numAlbums) {
            for (int i = index; i < numAlbums - 1; i++)
                albums[i] = albums[i + 1];
            numAlbums--;
            cout << "Album deleted.\n";
        } else {
            cout << "Invalid index.\n";
        }
    }

    void editAlbumTitle(int index, string newTitle) {
        if (index >= 0 && index < numAlbums) {
            albums[index].setTitle(newTitle);
            cout << "Album title updated.\n";
        } else {
            cout << "Invalid index.\n";
        }
    }

    void addSongToAlbum(int index, const Song& s) {
        if (index >= 0 && index < numAlbums)
            albums[index].addSong(s);
        else
            cout << "Invalid album index.\n";
    }

    void deleteSong(int a, int s) {
        if (a >= 0 && a < numAlbums)
            albums[a].deleteSong(s);
        else
            cout << "Invalid album index.\n";
    }

    void editSong(int a, int s) {
        if (a >= 0 && a < numAlbums)
            albums[a].editSong(s);
        else
            cout << "Invalid album index.\n";
    }

    void display() const {
        cout << "\n=== MUSIC LIBRARY ===\n";
        for (int i = 0; i < numAlbums; i++) {
            cout << "\n===== ALBUM #" << i << " =====\n";
            albums[i].display();
        }
    }

    // -------- SAVE --------
    void saveToFile(const string& filename) {
        ofstream file(filename);
        if (!file) return;

        file << numAlbums << endl;

        for (int i = 0; i < numAlbums; i++) {
            file << albums[i].getTitle() << endl;

            int ns = albums[i].getNumSongs();
            file << ns << endl;

            for (int j = 0; j < ns; j++) {
                Song s = albums[i].getSong(j);

                file << s.getTitle() << endl;
                file << s.getArtist().name << endl;
                file << s.getArtist().genre << endl;
                file << s.getArtist().yearBorn << endl;
                file << s.getDuration() << endl;
            }
        }

        cout << "Saved.\n";
    }

    // -------- LOAD --------
    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cout << "No previous library.\n";
            return;
        }

        numAlbums = 0;
        int albumCount;
        file >> albumCount;
        file.ignore();

        for (int i = 0; i < albumCount; i++) {
            char title[200];
            file.getline(title, 200);
            addAlbum(title);

            int songCount;
            file >> songCount;
            file.ignore();

            for (int j = 0; j < songCount; j++) {
                char st[200], name[200], gen[200];
                int y;
                double d;

                file.getline(st, 200);
                file.getline(name, 200);
                file.getline(gen, 200);
                file >> y;
                file >> d;
                file.ignore();

                Artist a = {name, gen, y};
                Song s(st, a, d);

                albums[i].addSong(s);
            }
        }

        cout << "Loaded.\n";
    }
};

// ====================== MAIN ======================
int main() {
    MusicLibrary lib;
    lib.loadFromFile("library.txt");

    int choice;

    do {
        cout << "\n=== MENU ===\n"
             << "1. Add Album\n"
             << "2. Add Song\n"
             << "3. Edit Album Title\n"
             << "4. Delete Album\n"
             << "5. Edit Song\n"
             << "6. Delete Song\n"
             << "7. Display Library\n"
             << "8. Save\n"
             << "9. Exit\n"
             << "Choice: ";

        cin >> choice;

        if (choice == 1) {
            char title[100];
            cin.ignore();
            cout << "Enter album title: ";
            cin.getline(title, 100);
            lib.addAlbum(title);
        }
        else if (choice == 2) {
            int index;
            cout << "Album index: ";
            cin >> index;
            cin.ignore();

            char t[100], name[100], gen[100];
            int y;
            double d;

            cout << "Song title: ";
            cin.getline(t, 100);
            cout << "Artist name: ";
            cin.getline(name, 100);
            cout << "Genre: ";
            cin.getline(gen, 100);
            cout << "Year born: ";
            cin >> y;
            cout << "Duration: ";
            cin >> d;

            Artist a = {name, gen, y};
            Song s(t, a, d);
            lib.addSongToAlbum(index, s);
        }
        else if (choice == 3) {
            int index;
            char title[100];
            cout << "Album index: ";
            cin >> index;
            cin.ignore();
            cout << "New album title: ";
            cin.getline(title, 100);
            lib.editAlbumTitle(index, title);
        }
        else if (choice == 4) {
            int index;
            cout << "Album index: ";
            cin >> index;
            lib.deleteAlbum(index);
        }
        else if (choice == 5) {
            int a, s;
            cout << "Album index: ";
            cin >> a;
            cout << "Song index: ";
            cin >> s;
            lib.editSong(a, s);
        }
        else if (choice == 6) {
            int a, s;
            cout << "Album index: ";
            cin >> a;
            cout << "Song index: ";
            cin >> s;
            lib.deleteSong(a, s);
        }
        else if (choice == 7)
            lib.display();

        else if (choice == 8)
            lib.saveToFile("library.txt");

    } while (choice != 9);

    return 0;
}
