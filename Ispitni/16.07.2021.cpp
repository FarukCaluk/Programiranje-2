#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <mutex>
#include <thread>
using namespace std;

const char* crt = "\n-------------------------------------------\n";
enum Pojas { BIJELI, ZUTI, NARANDZASTI, ZELENI, PLAVI, SMEDJI, CRNI };
const char* ispisPojasa[] = { "BIJELI", "ZUTI", "NARANDZASTI", "ZELENI", "PLAVI", "SMEDJI", "CRNI" };
const int brojTehnika = 6;
const char* NIJE_VALIDNA = "<VRIJEDNOST_NIJE_VALIDNA>";

char* GetNizKaraktera(const char* sadrzaj) {
	if (sadrzaj == nullptr)return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}
template<class T1, class T2, int max = 10>
class Kolekcija {
	T1* _elementi1[max] = { nullptr };
	T2* _elementi2[max] = { nullptr };
	int* _trenutno;
public:
	Kolekcija() {
		_trenutno = new int();
	}
	Kolekcija(const Kolekcija& obj)
	{
		_trenutno = new int(*obj._trenutno);
		for (size_t i = 0; i < *obj._trenutno; i++)
		{
			_elementi1[i] = new T1(*obj._elementi1[i]);
			_elementi2[i] = new T2(*obj._elementi2[i]);
		}
	}
	Kolekcija& operator=(const Kolekcija& obj)
	{
		if (this != &obj)
		{
			for (size_t i = 0; i < *_trenutno; i++) {
				delete _elementi1[i]; _elementi1[i] = nullptr;
				delete _elementi2[i]; _elementi2[i] = nullptr;
			}
			delete _trenutno; _trenutno = nullptr;
			_trenutno = new int(*obj._trenutno);
			for (size_t i = 0; i < *obj._trenutno; i++)
			{
				_elementi1[i] = new T1(*obj._elementi1[i]);
				_elementi2[i] = new T2(*obj._elementi2[i]);
			}
		}
		return *this;
	}
	~Kolekcija() {
		for (size_t i = 0; i < *_trenutno; i++) {
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
		delete _trenutno; _trenutno = nullptr;
	}
	void AddElement(T1 e1, T2 e2)
	{
		if (*_trenutno == max)
			throw exception("MAX BROJ ELEMENATA!");
		_elementi1[*_trenutno] = new T1(e1);
		_elementi2[*_trenutno] = new T2(e2);
		(*_trenutno)++;
	}
	void RemoveAt(int lokacija)
	{
		delete _elementi1[lokacija];
		delete _elementi2[lokacija];
		for (size_t i = lokacija; i < *_trenutno-1; i++)
		{
			_elementi1[i] = _elementi1[i + 1];
			_elementi2[i] = _elementi2[i + 1];
		}
		(*_trenutno)--;
	}
	void AddElement(T1 e1, T2 e2, int lokacija)
	{
		if (*_trenutno == max)
			throw exception("DOSEGNUT  JE MAXIMUM!");
		for (size_t i = *_trenutno; i > lokacija; i--)
		{
			_elementi1[i] = _elementi1[i - 1];
			_elementi2[i] = _elementi2[i - 1];
		}
		_elementi1[lokacija] = new T1(e1);
		_elementi2[lokacija] = new T2(e2);
		(*_trenutno)++;
	}
	T2& operator[](T1 e1)
	{
		for (size_t i = 0; i < *_trenutno; i++)
		{
			if (*_elementi1[i] == e1)
				return *_elementi2[i];
		}
	}
	T1& getElement1(int lokacija)const { return *_elementi1[lokacija]; }
	T2& getElement2(int lokacija)const { return *_elementi2[lokacija]; }
	int getTrenutno()const { return *_trenutno; }
	friend ostream& operator<< (ostream& COUT, const Kolekcija& obj) {
		for (size_t i = 0; i < *obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}
};
class Datum {
	int* _dan, * _mjesec, * _godina;
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	Datum(const Datum& obj) {
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
	}
	Datum& operator=(const Datum& obj) {
		if (this != &obj)
		{
			delete _dan; _dan = nullptr;
			delete _mjesec; _mjesec = nullptr;
			delete _godina; _godina = nullptr;
			_dan = new int(*obj._dan);
			_mjesec = new int(*obj._mjesec);
			_godina = new int(*obj._godina);
		}
		return *this;
	}
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	friend ostream& operator<< (ostream& COUT, const Datum& obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
		return COUT;
	}
	int toDays()const 
	{
		return *_dan + *_mjesec * 30 + *_godina * 365;
	}
};

class Tehnika {
	char* _naziv;
	//int se odnosi na ocjenu u opsegu od 1 � 5, a Datum na datum kada je ocijenjena odredjena tehnika
	Kolekcija<int, Datum*, brojTehnika> _ocjene;
public:
	Tehnika(const char* naziv) {
		_naziv = GetNizKaraktera(naziv);
	}
	Tehnika(const Tehnika& obj)
	{
		_naziv = GetNizKaraktera(obj._naziv);
		_ocjene = obj._ocjene;
	}
	Tehnika& operator=(const Tehnika& obj)
	{
		if (this != &obj)
		{
			delete[] _naziv; _naziv = nullptr;
			_naziv = GetNizKaraktera(obj._naziv);
			_ocjene = obj._ocjene;
		}
		return *this;
	}
	float prosjekTehnika()const
	{
		if (_ocjene.getTrenutno() == 0)
			return 0;
		float prosjek = 0;
		for (size_t i = 0; i < _ocjene.getTrenutno(); i++)
		{
			prosjek += _ocjene.getElement1(i);
		}
		return prosjek / _ocjene.getTrenutno();
	}
	bool operator==(const Tehnika& obj)
	{
		return strcmp(_naziv, obj._naziv) == 0;
	}
	~Tehnika() {
		delete[] _naziv; _naziv = nullptr;
	}
	
	bool AddOcjena(int ocjena,Datum date)
	{
		for (size_t i = 0; i < _ocjene.getTrenutno(); i++)
		{
			int razlika = date.toDays() - _ocjene.getElement2(i)->toDays();

			if (razlika < 3)
				return false;
		}
		_ocjene.AddElement(ocjena, new Datum(date));
		return true;
	}
	char* GetNaziv() { return _naziv; }
	Kolekcija<int, Datum*, brojTehnika>& GetOcjene() { return _ocjene; }
	friend ostream& operator<< (ostream& COUT, const Tehnika& obj) {
		COUT << obj._naziv << endl;
		for (size_t i = 0; i < obj._ocjene.getTrenutno(); i++)
		{
			COUT << obj._ocjene.getElement1(i) << " - " << *obj._ocjene.getElement2(i) << endl;
		}
		COUT << "Prosjek: " << obj.prosjekTehnika() << endl;
		return COUT;
	}
};

class Polaganje {
	Pojas _pojas;
	vector<Tehnika*> _polozeneTehnike;
public:
	Polaganje(Pojas pojas = BIJELI) {
		_pojas = pojas;
	}
	Polaganje(const Polaganje& obj)
	{
		_pojas = obj._pojas;
		for (size_t i = 0; i < obj._polozeneTehnike.size(); i++)
		{
			_polozeneTehnike.push_back(new Tehnika(*obj._polozeneTehnike[i]));
		}
	}
	Polaganje& operator=(const Polaganje& obj)
	{
		if (this != &obj)
		{
			for (size_t i = 0; i < _polozeneTehnike.size(); i++) {
				delete _polozeneTehnike[i];
				_polozeneTehnike[i] = nullptr;
			}
			_pojas = obj._pojas;
			for (size_t i = 0; i < obj._polozeneTehnike.size(); i++)
			{
				_polozeneTehnike.push_back(new Tehnika(*obj._polozeneTehnike[i]));
			}
		}
		return *this;
	}
	float prosjekPojas()const
	{
		if (_polozeneTehnike.size() == 0)
			return 0;
		float prosjek = 0;
		for (size_t i = 0; i < _polozeneTehnike.size(); i++)
		{
			prosjek += _polozeneTehnike[i]->prosjekTehnika();
		}
		return prosjek / _polozeneTehnike.size();
	}
	~Polaganje() {
		for (size_t i = 0; i < _polozeneTehnike.size(); i++) {
			delete _polozeneTehnike[i];
			_polozeneTehnike[i] = nullptr;
		}
	}
	vector<Tehnika*>& GetTehnike() { return _polozeneTehnike; }
	Pojas GetPojas() { return _pojas; }
	friend ostream& operator<< (ostream& COUT, const Polaganje& obj) {
		COUT << obj._pojas << endl;
		for (size_t i = 0; i < obj._polozeneTehnike.size(); i++)
			COUT << *obj._polozeneTehnike[i];
		return COUT;
	}
};
bool ValidirajLozinku(string lozinka)
{
	return regex_search(lozinka, regex(".{7,}")) &&
		regex_search(lozinka, regex("[A-Z]")) &&
		regex_search(lozinka, regex("[a-z]")) &&
		regex_search(lozinka, regex("[0-9]")) &&
		regex_search(lozinka, regex("[@*-]"));
}
class Korisnik {
	char* _imePrezime;
	string _emailAdresa;
	string _lozinka;
public:
	Korisnik(const char* imePrezime, string emailAdresa, string lozinka)
	{
		_imePrezime = GetNizKaraktera(imePrezime);
		_emailAdresa = emailAdresa;
		_lozinka = ValidirajLozinku(lozinka) ? lozinka : NIJE_VALIDNA;	
	}
	Korisnik(const Korisnik& obj)
	{
		_imePrezime = GetNizKaraktera(obj._imePrezime);
		_emailAdresa = obj._emailAdresa;
		_lozinka = obj._lozinka;
	}
	Korisnik& operator=(const Korisnik& obj)
	{
		if (this != &obj)
		{
			delete[] _imePrezime; _imePrezime = nullptr;
			_imePrezime = GetNizKaraktera(obj._imePrezime);
			_emailAdresa = obj._emailAdresa;
			_lozinka = obj._lozinka;
		}
		return *this;
	}
	~Korisnik() { delete[] _imePrezime; _imePrezime = nullptr; }
	string GetEmail() { return _emailAdresa; }
	string GetLozinka() { return _lozinka; }
	char* GetImePrezime() { return _imePrezime; }
	virtual void info() = 0;
};
mutex mtx;
class KaratePolaznik:public Korisnik {
	vector<Polaganje*> _polozeniPojasevi;
public:
	KaratePolaznik(const char* imePrezime, string emailAdresa, string
		lozinka) :Korisnik(imePrezime,emailAdresa,lozinka){
	}
	KaratePolaznik(const KaratePolaznik& obj):Korisnik(obj) 
	{
		for (size_t i = 0; i < obj._polozeniPojasevi.size(); i++) 
		{
			_polozeniPojasevi.push_back(new Polaganje(*obj._polozeniPojasevi[i])); 
		}
	}
	KaratePolaznik& operator=(const KaratePolaznik& obj)
	{
		if (this != &obj)
		{ 
			for (size_t i = 0; i < _polozeniPojasevi.size(); i++) { 
				delete _polozeniPojasevi[i];
				_polozeniPojasevi[i] = nullptr;
			}
			for (size_t i = 0; i < obj._polozeniPojasevi.size(); i++) 
			{ 
				_polozeniPojasevi.push_back(new Polaganje(*obj._polozeniPojasevi[i])); 
			}
		}
		return *this;
	}
	float UkupniProsjek()const
	{
		if (_polozeniPojasevi.size() == 0)
			return 0;
		float prosjek = 0;
		for (size_t i = 0; i < _polozeniPojasevi.size(); i++)
		{
			prosjek += _polozeniPojasevi[i]->prosjekPojas();
		}
		return prosjek / _polozeniPojasevi.size();
	}
	~KaratePolaznik() {
		cout << crt << "DESTRUKTOR -> KaratePolaznik" << crt;
		for (size_t i = 0; i < _polozeniPojasevi.size(); i++) {
			delete _polozeniPojasevi[i];
			_polozeniPojasevi[i] = nullptr;
		}
	}
	void PosaljiMail(Tehnika tehnika,Pojas pojas,float prosjek)
	{
		mtx.lock();
		cout << "FROM:info@karate.ba\n";
		cout << "TO: " << GetEmail() << endl;
		cout << "Postovani: " <<GetImePrezime()<<", evidentirana vam je thenika "<<tehnika<<" za pojas\n";
		cout << "Dosadasnji uspjeh (prosjek ocjena)\n " <<"na pojasu "<<pojas<<" iznosi "<<prosjek<<", a ukupni uspjeh (prosjek ocjena) na svim pojasevima ";
		cout << "iznosi " << UkupniProsjek() << ".\n";
		cout << "Fakultet informacijskih tehnologija\n";
		cout << "Pismeni dio ispita\n";
		cout << "Pozdrav.\nKARATE Team.\n";
		mtx.unlock();
	}
	/*AddTehniku
	svi kandidati podrazumijevano imaju BIJELI pojas (za njega se ne dodaju tehnike)
	sve tehnike na nivou jednog pojasa (ZUTI, ZELENI ... ) se evidentiraju
   unutar istog objekta tipa Polaganje,
	tom prilikom onemoguciti:
	- dodavanje istih (moraju biti identicne vrijednosti svih atributa)
   tehnika na nivou jednog pojasa,
	- dodavanje tehnika za visi pojas ako prethodni pojas nema evidentirane
   najmanje 3 tehnike ili nema prosjecnu ocjenu svih tehnika vecu od 3.5
	(onemoguciti dodavanje tehnike za NARANDZASTI ako ne postoji najmanje 3
   tehnike za ZUTI pojas i njihov prosjek je veci od 3.5)
	funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
	*/
	bool AddTehniku(Pojas pojas,Tehnika tehnika)
	{
		if (pojas > ZUTI)
		{
			bool postojiNizPojasa = false;
			for (size_t i = 0; i < _polozeniPojasevi.size(); i++)
			{
				if (_polozeniPojasevi[i]->GetPojas() == pojas - 1) 
				{
					postojiNizPojasa = true;
					if (_polozeniPojasevi[i]->GetTehnike().size() < 3)
						return false;

					for (size_t j = 0; j < _polozeniPojasevi[i]->GetTehnike().size(); j++)
					{
						if (_polozeniPojasevi[i]->GetTehnike()[j]->prosjekTehnika() < 3.5)
							return false;
					}
				}
			}
			if (!postojiNizPojasa) 
				return false;
		}
		for (size_t i = 0; i < _polozeniPojasevi.size(); i++)
		{
			if (_polozeniPojasevi[i]->GetPojas() == pojas) {
				for (size_t j = 0; j < _polozeniPojasevi[i]->GetTehnike().size(); j++)
				{
					if (*_polozeniPojasevi[i]->GetTehnike()[j] == tehnika)
						return false;
				}
				_polozeniPojasevi[i]->GetTehnike().push_back(new Tehnika(tehnika));

				thread trd(&KaratePolaznik::PosaljiMail, this, tehnika, pojas, _polozeniPojasevi[i]->prosjekPojas());
				trd.join();

				return true;
			}
		}
		Polaganje* polaganje = new Polaganje(pojas);
		polaganje->GetTehnike().push_back(new Tehnika(tehnika));
		_polozeniPojasevi.push_back(polaganje);

		thread trd(&KaratePolaznik::PosaljiMail, this, tehnika, pojas, polaganje->prosjekPojas());
		trd.join();
		return true;
	}
	friend ostream& operator<< (ostream& COUT, KaratePolaznik& obj) {
		COUT << obj.GetImePrezime() << " " << obj.GetEmail() << " " <<
			obj.GetLozinka() << endl;
		for (size_t i = 0; i < obj._polozeniPojasevi.size(); i++)
			COUT << *obj._polozeniPojasevi[i];
		return COUT;
	}
	vector<Polaganje*>& GetPolozeniPojasevi() { return _polozeniPojasevi; }
	void info() {}
};
const char* GetOdgovorNaPrvoPitanje() {
	cout << "Pitanje -> Pojasnite ulogu operatora const_cast?\n";
	return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}

void main() {
	cout << GetOdgovorNaPrvoPitanje() << endl;
	cin.get();

	Datum datum19062020(19, 6, 2020),
		datum20062020(20, 6, 2020),
		datum30062020(30, 6, 2020),
		datum05072020(5, 7, 2020);

	int kolekcijaTestSize = 10;
	Kolekcija<int, int> kolekcija1;
	for (int i = 0; i < kolekcijaTestSize; i++)
		kolekcija1.AddElement(i, i);
	cout << kolekcija1 << endl;
	try {
		kolekcija1.AddElement(11, 11);
	}
	catch (exception& err) {
		cout << crt << "Greska -> " << err.what() << crt;
	}
	cout << kolekcija1 << crt;

	kolekcija1.RemoveAt(2);
	
	cout << kolekcija1 << crt;
	kolekcija1.AddElement(9, 9, 2);
	
	cout << kolekcija1 << crt;
	Kolekcija<int, int> kolekcija2 = kolekcija1; 
	cout << kolekcija1 << crt;
	
	kolekcija1[9] = 2;
	
	Tehnika choku_zuki("choku_zuki"),
		gyaku_zuki("gyaku_zuki"),
		kizami_zuki("kizami_zuki"),
		oi_zuki("oi_zuki");
	
	if (choku_zuki.AddOcjena(1, datum19062020))
		cout << "Ocjena evidentirana!" << endl;
	if (!choku_zuki.AddOcjena(5, datum20062020))
		cout << "Ocjena NIJE evidentirana!" << endl;
	if (choku_zuki.AddOcjena(5, datum30062020))
		cout << "Ocjena evidentirana!" << endl;
	
	cout << choku_zuki << endl;
	if (ValidirajLozinku("john4Do*e"))
		cout << "OK" << crt;
	if (!ValidirajLozinku("john4Doe"))
		cout << "Specijalni znak?" << crt;
	if (!ValidirajLozinku("jo*4Da"))
		cout << "7 znakova?" << crt;
	if (!ValidirajLozinku("4jo-hnoe"))
		cout << "Veliko slovo?" << crt;
	if (ValidirajLozinku("@john2Doe"))
		cout << "OK" << crt;
	
	Korisnik* jasmin = new KaratePolaznik("Jasmin Azemovic",
		"jasmin@karate.ba", "j@sm1N*");
	Korisnik* adel = new KaratePolaznik("Adel Handzic", "adel@edu.karate.ba",
		"4Ade1*H");
	Korisnik* lozinkaNijeValidna = new KaratePolaznik("John Doe",
		"john.doe@google.com", "johndoe"); 
	/*AddTehniku
	svi kandidati podrazumijevano imaju BIJELI pojas (za njega se ne dodaju
   tehnike)
	sve tehnike na nivou jednog pojasa (ZUTI, ZELENI ... ) se evidentiraju
   unutar istog objekta tipa Polaganje,
	tom prilikom onemoguciti: 
	- dodavanje istih (moraju biti identicne vrijednosti svih atributa) 
   tehnika na nivou jednog pojasa, 
	- dodavanje tehnika za visi pojas ako prethodni pojas nema evidentirane 
   najmanje 3 tehnike ili nema prosjecnu ocjenu svih tehnika vecu od 3.5 
	(onemoguciti dodavanje tehnike za NARANDZASTI ako ne postoji najmanje 3 
   tehnike za ZUTI pojas i njihov prosjek je veci od 3.5) 
	funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
	*/
	//doraditi klase da nacin da omoguce izvrsenje naredne linije koda
	KaratePolaznik* jasminPolaznik = dynamic_cast<KaratePolaznik*>(jasmin);
	if (jasminPolaznik != nullptr) {
		if (jasminPolaznik->AddTehniku(ZUTI, gyaku_zuki))
			cout << "Tehnika uspjesno dodan!" << crt;
		//ne treba dodati kizami_zuki jer ne postoje evidentirane 3 tehnike za ZUTI pojas
		if (!jasminPolaznik->AddTehniku(NARANDZASTI, kizami_zuki))
			cout << "Tehnika NIJE uspjesno dodana!" << crt;
		if (jasminPolaznik->AddTehniku(ZUTI, kizami_zuki))
			cout << "Tehnika uspjesno dodan!" << crt;
		if (jasminPolaznik->AddTehniku(ZUTI, oi_zuki))
			cout << "Tehnika uspjesno dodan!" << crt;
		if (jasminPolaznik->AddTehniku(ZUTI, choku_zuki))
			cout << "Tehnika uspjesno dodan!" << crt;
		//ne treba dodati choku_zuki jer je vec dodana za zuti pojas
		if (!jasminPolaznik->AddTehniku(ZUTI, choku_zuki))
			cout << "Tehnika NIJE uspjesno dodana!" << crt;
		//ispisuje sve dostupne podatke o karate polazniku
		cout << *jasminPolaznik << crt;
	}
	
	//osigurati da se u narednim linijama poziva i destruktor klase KaratePolaznik
	delete jasmin;
	delete adel;
	delete lozinkaNijeValidna;
	cin.get();
	system("pause>0");
}