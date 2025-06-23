Arborele lui Sobo (15p)
Sobo locuiește într-un parc sub formă de arbore, cu rădăcina în nodul 0, unde se află casa lui. Fiecare nod reprezintă un restaurant.

Restaurantele sunt păzite de pisici, iar lui Sobo îi este teamă să treacă pe un drum care conține mai mult de m astfel de restaurante (mai precis, mai mult de m restaurante consecutive cu pisici de pază).

Un restaurant potrivit pentru Sobo este un restaurant frunză — adică un restaurant care nu mai duce nicăieri (nu are niciun copil).

Sobo vrea să știe câte astfel de restaurante poate vizita, astfel încât drumul de la casa lui (nodul 0) până la acel restaurant să nu conțină mai mult de m restaurante consecutive cu pisici.

Input
Prima linie conține două numere întregi: n — numărul de noduri din arbore (2 ≤ n ≤ 200), m — numărul maxim de restaurante consecutive cu pisici prin care Sobo poate trece (1 ≤ m ≤ n)
A doua linie conține n valori: a0, a1, ..., a_{n-1}, unde:
ai = 1 dacă nodul i are o pisică de pază
ai = 0 dacă nodul i nu are pisică
Următoarele n - 1 linii conțin câte două numere întregi u v (0 ≤ u, v < n, u ≠ v), reprezentând muchiile arborelui.