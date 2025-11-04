#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <iomanip>
#include <sstream>
#include <vector>
#include <algorithm>


using namespace std;

struct KhachHang {
    int soThuTu;
    string cmnd;
    string hoTen;
    KhachHang* next;

    KhachHang() : soThuTu(0), next(NULL) {}
};

struct Ve {
    string maVe;
    string maChuyenBay;
    string cmnd;
    string hoTen;
    int soGhe;
    Ve* next;

    Ve() : soGhe(0), next(NULL) {}
};

struct NodeGhe {
    int soGhe;
    NodeGhe* next;

    NodeGhe() : soGhe(0), next(NULL) {}
};

struct ChuyenBay {
    string maChuyenBay;
    string soHieuMayBay;
    string ngayKhoiHanh;
    string sanBayDen;
    int trangThai;
    Ve* danhSachVe;
    NodeGhe* danhSachGheTrong;
    ChuyenBay* left;
    ChuyenBay* right;

    ChuyenBay() : trangThai(1), danhSachVe(NULL), danhSachGheTrong(NULL),
                  left(NULL), right(NULL) {}
};

struct MayBay {
    string soHieuMayBay;
    int soCho;
    MayBay* next;

    MayBay() : soCho(0), next(NULL) {}
};

MayBay* danhSachMayBay = NULL;
ChuyenBay* cayChuyenBay = NULL;
KhachHang* danhSachKhachHang = NULL;

void timVe(ChuyenBay* root, string maVe, ChuyenBay*& cbResult, Ve*& veResult);
void thongKeGheTrong();
void hienThiKhachHangTheoChuyenBay(ChuyenBay* cb);

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void clearScreen() {
    system("cls");
}

string trim(string str) {
    size_t first = str.find_first_not_of(' ');
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

string nhapMatKhau() {
    string password = "";
    char ch;
    while (true) {
        ch = _getch();
        if (ch == 13) break;
        if (ch == 27) return "ESC";
        if (ch == 8) {
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b";
            }
        } else {
            password += ch;
            cout << "*";
        }
    }
    cout << endl;
    return password;
}

void themMayBay(MayBay*& head, string soHieu, int soCho) {
    MayBay* mb = new MayBay();
    mb->soHieuMayBay = soHieu;
    mb->soCho = soCho;
    mb->next = head;
    head = mb;
}

MayBay* timMayBay(MayBay* head, string soHieu) {
    MayBay* p = head;
    while (p != NULL) {
        if (p->soHieuMayBay == soHieu) return p;
        p = p->next;
    }
    return NULL;
}

void docFileMayBay() {
    ifstream file("MayBay.txt");
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string soHieu;
        int soCho;
        getline(ss, soHieu, ',');
        ss >> soCho;
        soHieu = trim(soHieu);
        themMayBay(danhSachMayBay, soHieu, soCho);
    }
    file.close();
}

void ghiFileMayBay() {
    ofstream file("MayBay.txt");
    MayBay* p = danhSachMayBay;
    while (p != NULL) {
        file << p->soHieuMayBay << "," << p->soCho << endl;
        p = p->next;
    }
    file.close();
}

void themGhe(NodeGhe*& head, int soGhe) {
    NodeGhe* ghe = new NodeGhe();
    ghe->soGhe = soGhe;
    ghe->next = head;
    head = ghe;
}

void xoaGhe(NodeGhe*& head, int soGhe) {
    if (head == NULL) return;

    if (head->soGhe == soGhe) {
        NodeGhe* temp = head;
        head = head->next;
        delete temp;
        return;
    }

    NodeGhe* p = head;
    while (p->next != NULL && p->next->soGhe != soGhe) {
        p = p->next;
    }
    if (p->next != NULL) {
        NodeGhe* temp = p->next;
        p->next = p->next->next;
        delete temp;
    }
}

int demGheTrong(NodeGhe* head) {
    int count = 0;
    NodeGhe* p = head;
    while (p != NULL) {
        count++;
        p = p->next;
    }
    return count;
}

void themVe(Ve*& head, Ve* veNew) {
    veNew->next = head;
    head = veNew;
}

void xoaVe(Ve*& head, string maVe) {
    if (head == NULL) return;

    if (head->maVe == maVe) {
        Ve* temp = head;
        head = head->next;
        delete temp;
        return;
    }

    Ve* p = head;
    while (p->next != NULL && p->next->maVe != maVe) {
        p = p->next;
    }
    if (p->next != NULL) {
        Ve* temp = p->next;
        p->next = p->next->next;
        delete temp;
    }
}

void ghiFileVe(Ve* ve) {
    if (ve == NULL) return;
    ofstream file(ve->maVe + ".txt");
    file << "Ma ve: " << ve->maVe << endl;
    file << "Ma chuyen bay: " << ve->maChuyenBay << endl;
    file << "CMND: " << ve->cmnd << endl;
    file << "Ho ten: " << ve->hoTen << endl;
    file << "So ghe: " << ve->soGhe << endl;
    file.close();
}

void xoaFileVe(string maVe) {
    string filename = maVe + ".txt";
    remove(filename.c_str());
}

void themChuyenBay(ChuyenBay*& root, ChuyenBay* cbNew) {
    if (root == NULL) {
        root = cbNew;
        return;
    }

    if (cbNew->maChuyenBay < root->maChuyenBay) {
        themChuyenBay(root->left, cbNew);
    } else {
        themChuyenBay(root->right, cbNew);
    }
}

ChuyenBay* timChuyenBay(ChuyenBay* root, string maChuyenBay) {
    if (root == NULL) return NULL;
    if (root->maChuyenBay == maChuyenBay) return root;
    if (maChuyenBay < root->maChuyenBay)
        return timChuyenBay(root->left, maChuyenBay);
    return timChuyenBay(root->right, maChuyenBay);
}

void duyetChuyenBay(ChuyenBay* root, void (*func)(ChuyenBay*)) {
    if (root == NULL) return;
    duyetChuyenBay(root->left, func);
    func(root);
    duyetChuyenBay(root->right, func);
}

void capNhatTrangThaiChuyenBay(ChuyenBay* cb) {
    if (cb == NULL) return;
    if (cb->trangThai == 0 || cb->trangThai == 3) return;

    int soGheTrong = demGheTrong(cb->danhSachGheTrong);
    if (soGheTrong == 0) {
        cb->trangThai = 2;
    } else {
        cb->trangThai = 1;
    }
}

void docFileChuyenBay() {
    ifstream file("ChuyenBay.txt");
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        ChuyenBay* cb = new ChuyenBay();
        stringstream ss(line);

        getline(ss, cb->maChuyenBay, ',');
        getline(ss, cb->soHieuMayBay, ',');
        getline(ss, cb->ngayKhoiHanh, ',');
        getline(ss, cb->sanBayDen, ',');
        ss >> cb->trangThai;

        cb->maChuyenBay = trim(cb->maChuyenBay);
        cb->soHieuMayBay = trim(cb->soHieuMayBay);
        cb->ngayKhoiHanh = trim(cb->ngayKhoiHanh);
        cb->sanBayDen = trim(cb->sanBayDen);

        MayBay* mb = timMayBay(danhSachMayBay, cb->soHieuMayBay);
        if (mb != NULL) {
            for (int i = mb->soCho; i >= 1; i--) {
                themGhe(cb->danhSachGheTrong, i);
            }
        }

        themChuyenBay(cayChuyenBay, cb);
    }
    file.close();
}

void ghiFileChuyenBayRecursive(ChuyenBay* root, ofstream& file) {
    if (root == NULL) return;
    ghiFileChuyenBayRecursive(root->left, file);
    file << root->maChuyenBay << "," << root->soHieuMayBay << ","
         << root->ngayKhoiHanh << "," << root->sanBayDen << ","
         << root->trangThai << endl;
    ghiFileChuyenBayRecursive(root->right, file);
}

void ghiFileChuyenBay() {
    ofstream file("ChuyenBay.txt");
    ghiFileChuyenBayRecursive(cayChuyenBay, file);
    file.close();
}

void themKhachHang(KhachHang*& head, string cmnd, string hoTen) {
    KhachHang* kh = new KhachHang();

    int maxSTT = 0;
    KhachHang* p = head;
    while (p != NULL) {
        if (p->soThuTu > maxSTT) maxSTT = p->soThuTu;
        p = p->next;
    }

    kh->soThuTu = maxSTT + 1;
    kh->cmnd = cmnd;
    kh->hoTen = hoTen;
    kh->next = head;
    head = kh;
}

void xoaKhachHang(KhachHang*& head, int soThuTu) {
    if (head == NULL) return;

    if (head->soThuTu == soThuTu) {
        KhachHang* temp = head;
        head = head->next;
        delete temp;
        return;
    }

    KhachHang* p = head;
    while (p->next != NULL && p->next->soThuTu != soThuTu) {
        p = p->next;
    }
    if (p->next != NULL) {
        KhachHang* temp = p->next;
        p->next = p->next->next;
        delete temp;
    }
}

void docFileKhachHang() {
    ifstream file("KhachHang.txt");
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);

        KhachHang* kh = new KhachHang();
        ss >> kh->soThuTu;
        ss.ignore();
        getline(ss, kh->cmnd, ',');
        getline(ss, kh->hoTen);

        kh->cmnd = trim(kh->cmnd);
        kh->hoTen = trim(kh->hoTen);
        kh->next = danhSachKhachHang;
        danhSachKhachHang = kh;
    }
    file.close();
}

void ghiFileKhachHang() {
    ofstream file("KhachHang.txt");
    KhachHang* p = danhSachKhachHang;
    while (p != NULL) {
        file << p->soThuTu << "," << p->cmnd << "," << p->hoTen << endl;
        p = p->next;
    }
    file.close();
}

string getTrangThaiStr(int trangThai) {
    switch (trangThai) {
        case 0: return "Huy chuyen";
        case 1: return "Con ve";
        case 2: return "Het ve";
        case 3: return "Hoan tat";
        default: return "Khong xac dinh";
    }
}

void hienThiChuyenBay(ChuyenBay* cb) {
    int soVe = 0;
    Ve* v = cb->danhSachVe;
    while (v != NULL) {
        soVe++;
        v = v->next;
    }

    cout << left << setw(15) << cb->maChuyenBay
         << setw(15) << cb->soHieuMayBay
         << setw(15) << cb->ngayKhoiHanh
         << setw(20) << cb->sanBayDen
         << setw(15) << getTrangThaiStr(cb->trangThai)
         << setw(10) << soVe << endl;
}

void hienThiDanhSachChuyenBay() {
    clearScreen();
    setColor(14);
    cout << "\n========== DANH SACH CHUYEN BAY ==========\n\n";
    setColor(11);

    cout << left << setw(15) << "Ma CB"
         << setw(15) << "So hieu MB"
         << setw(15) << "Ngay KH"
         << setw(20) << "San bay den"
         << setw(15) << "Trang thai"
         << setw(10) << "So ve" << endl;

    setColor(8);
    cout << string(90, '-') << endl;
    setColor(7);

    duyetChuyenBay(cayChuyenBay, hienThiChuyenBay);

    cout << endl;
}

void datVe() {
    clearScreen();
    setColor(14);
    cout << "\n========== DAT VE ==========\n\n";
    setColor(7);

    string maChuyenBay, cmnd, hoTen;
    int soGhe;

    cout << "Nhap ma chuyen bay: ";
    getline(cin, maChuyenBay);
    maChuyenBay = trim(maChuyenBay);

    if (maChuyenBay.empty()) {
        setColor(12);
        cout << "Loi: Ma chuyen bay khong duoc de trong!\n";
        setColor(7);
        return;
    }

    ChuyenBay* cb = timChuyenBay(cayChuyenBay, maChuyenBay);
    if (cb == NULL) {
        setColor(12);
        cout << "Loi: Khong tim thay chuyen bay!\n";
        setColor(7);
        return;
    }

    if (cb->trangThai == 0) {
        setColor(12);
        cout << "Loi: Chuyen bay da bi huy!\n";
        setColor(7);
        return;
    }

    if (cb->trangThai == 2) {
        setColor(12);
        cout << "Loi: Chuyen bay da het ve!\n";
        setColor(7);
        return;
    }

    if (cb->trangThai == 3) {
        setColor(12);
        cout << "Loi: Chuyen bay da hoan tat!\n";
        setColor(7);
        return;
    }

    cout << "Nhap CMND: ";
    getline(cin, cmnd);
    cmnd = trim(cmnd);

    if (cmnd.empty()) {
        setColor(12);
        cout << "Loi: CMND khong duoc de trong!\n";
        setColor(7);
        return;
    }

    cout << "Nhap ho ten: ";
    getline(cin, hoTen);
    hoTen = trim(hoTen);

    if (hoTen.empty()) {
        setColor(12);
        cout << "Loi: Ho ten khong duoc de trong!\n";
        setColor(7);
        return;
    }

    cout << "Nhap so ghe: ";
    cin >> soGhe;
    cin.ignore();

    NodeGhe* ghe = cb->danhSachGheTrong;
    bool gheHopLe = false;
    while (ghe != NULL) {
        if (ghe->soGhe == soGhe) {
            gheHopLe = true;
            break;
        }
        ghe = ghe->next;
    }

    if (!gheHopLe) {
        setColor(12);
        cout << "Loi: Ghe khong hop le hoac da duoc dat!\n";
        setColor(7);
        return;
    }

    themKhachHang(danhSachKhachHang, cmnd, hoTen);
    ghiFileKhachHang();

    setColor(10);
    cout << "\nDat ve thanh cong! Vui long cho nhan vien xu ly.\n";
    setColor(7);
}

bool dangNhap() {
    clearScreen();
    int soLanThu = 0;

    while (soLanThu < 3) {
        setColor(14);
        cout << "\n* * * * * * * * * * * * * * * * * * * * *\n";
        cout << "*      DANG NHAP HE THONG             *\n";
        cout << "* * * * * * * * * * * * * * * * * * * * *\n\n";
        setColor(7);

        string user, password;
        cout << "User: ";
        getline(cin, user);
        user = trim(user);

        cout << "Password: ";
        password = nhapMatKhau();

        if (password == "ESC") {
            return false;
        }

        ifstream file("Admin.txt");
        if (!file.is_open()) {
            setColor(12);
            cout << "Loi: Khong the mo file Admin.txt!\n";
            setColor(7);
            Sleep(2000);
            return false;
        }

        bool found = false;
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string u, p;
            getline(ss, u, ',');
            getline(ss, p);
            u = trim(u);
            p = trim(p);

            if (u == user && p == password) {
                found = true;
                break;
            }
        }
        file.close();

        if (found) {
            setColor(10);
            cout << "\nDang nhap thanh cong!\n";
            setColor(7);
            Sleep(1000);
            return true;
        } else {
            soLanThu++;
            setColor(12);
            cout << "\nSai user hoac password! Con " << (3 - soLanThu) << " lan thu.\n";
            setColor(7);
            Sleep(2000);
            clearScreen();
        }
    }

    setColor(12);
    cout << "\nDa nhap sai 3 lan. Thoat khoi he thong!\n";
    setColor(7);
    Sleep(2000);
    return false;
}

void xuLyDatVe() {
    clearScreen();
    setColor(14);
    cout << "\n========== XU LY DAT VE ==========\n\n";
    setColor(7);

    if (danhSachKhachHang == NULL) {
        setColor(12);
        cout << "Khong co khach hang nao dang cho!\n";
        setColor(7);
        return;
    }

    setColor(11);
    cout << "Danh sach khach hang cho:\n";
    setColor(7);

    KhachHang* kh = danhSachKhachHang;
    while (kh != NULL) {
        cout << "STT: " << kh->soThuTu << " - CMND: " << kh->cmnd
             << " - Ho ten: " << kh->hoTen << endl;
        kh = kh->next;
    }

    int soThuTu;
    cout << "\nNhap so thu tu khach hang can xu ly: ";
    cin >> soThuTu;
    cin.ignore();

    kh = danhSachKhachHang;
    while (kh != NULL && kh->soThuTu != soThuTu) {
        kh = kh->next;
    }

    if (kh == NULL) {
        setColor(12);
        cout << "Khong tim thay khach hang!\n";
        setColor(7);
        return;
    }

    string maChuyenBay;
    int soGhe;

    cout << "Nhap ma chuyen bay: ";
    getline(cin, maChuyenBay);
    maChuyenBay = trim(maChuyenBay);

    ChuyenBay* cb = timChuyenBay(cayChuyenBay, maChuyenBay);
    if (cb == NULL) {
        setColor(12);
        cout << "Khong tim thay chuyen bay!\n";
        setColor(7);
        return;
    }

    if (cb->trangThai != 1) {
        setColor(12);
        cout << "Chuyen bay khong the dat ve!\n";
        setColor(7);
        return;
    }

    cout << "Nhap so ghe: ";
    cin >> soGhe;
    cin.ignore();

    NodeGhe* ghe = cb->danhSachGheTrong;
    bool found = false;
    while (ghe != NULL) {
        if (ghe->soGhe == soGhe) {
            found = true;
            break;
        }
        ghe = ghe->next;
    }

    if (!found) {
        setColor(12);
        cout << "Ghe khong hop le!\n";
        setColor(7);
        return;
    }

    Ve* veNew = new Ve();
    veNew->maVe = maChuyenBay + "_" + to_string(soGhe);
    veNew->maChuyenBay = maChuyenBay;
    veNew->cmnd = kh->cmnd;
    veNew->hoTen = kh->hoTen;
    veNew->soGhe = soGhe;

    themVe(cb->danhSachVe, veNew);
    xoaGhe(cb->danhSachGheTrong, soGhe);
    capNhatTrangThaiChuyenBay(cb);
    xoaKhachHang(danhSachKhachHang, soThuTu);

    ghiFileVe(veNew);
    ghiFileChuyenBay();
    ghiFileKhachHang();

    setColor(10);
    cout << "\nXu ly dat ve thanh cong! Ma ve: " << veNew->maVe << endl;
    setColor(7);
}

void timVe(ChuyenBay* root, string maVe, ChuyenBay*& cbResult, Ve*& veResult) {
    if (root == NULL) return;

    Ve* v = root->danhSachVe;
    while (v != NULL) {
        if (v->maVe == maVe) {
            cbResult = root;
            veResult = v;
            return;
        }
        v = v->next;
    }

    timVe(root->left, maVe, cbResult, veResult);
    timVe(root->right, maVe, cbResult, veResult);
}

void xuLyTraVe() {
    clearScreen();
    setColor(14);
    cout << "\n========== XU LY TRA VE ==========\n\n";
    setColor(7);

    string maVe;
    cout << "Nhap ma ve can tra: ";
    getline(cin, maVe);
    maVe = trim(maVe);

    ChuyenBay* cb = NULL;
    Ve* ve = NULL;
    timVe(cayChuyenBay, maVe, cb, ve);

    if (cb == NULL || ve == NULL) {
        setColor(12);
        cout << "Khong tim thay ve!\n";
        setColor(7);
        return;
    }

    if (cb->trangThai == 3) {
        setColor(12);
        cout << "Khong the tra ve! Chuyen bay da hoan tat.\n";
        setColor(7);
        return;
    }

    int soGhe = ve->soGhe;

    xoaVe(cb->danhSachVe, maVe);
    themGhe(cb->danhSachGheTrong, soGhe);
    capNhatTrangThaiChuyenBay(cb);
    xoaFileVe(maVe);
    ghiFileChuyenBay();

    setColor(10);
    cout << "\nTra ve thanh cong!\n";
    setColor(7);
}

void hienThiKhachHangTheoChuyenBay(ChuyenBay* cb) {
    if (cb == NULL) {
        cout << "Khong co chuyen bay.\n";
        return;
    }

    setColor(11);
    cout << "Danh sach khach hang dat ve tren chuyen bay " << cb->maChuyenBay << ":\n";
    setColor(7);

    Ve* ve = cb->danhSachVe;
    int stt = 1;
    if (ve == NULL) {
        cout << "Chua co khach hang nao dat ve!\n";
    } else {
        cout << left << setw(6) << "STT" << setw(12) << "Ma ve" << setw(15) << "CMND" << setw(25) << "Ho ten" << setw(6) << "Ghe" << endl;
        cout << string(65, '-') << endl;
        while (ve != NULL) {
            cout << left << setw(6) << stt
                 << setw(12) << ve->maVe
                 << setw(15) << ve->cmnd
                 << setw(25) << ve->hoTen
                 << setw(6) << ve->soGhe << endl;
            stt++;
            ve = ve->next;
        }
    }
}

void thongKeGheTrong() {
    clearScreen();
    setColor(14);
    cout << "\n========== DANH SACH GHE TRONG ==========\n\n";
    setColor(7);

    string maChuyenBay;
    cout << "Nhap ma chuyen bay: ";
    getline(cin, maChuyenBay);
    maChuyenBay = trim(maChuyenBay);

    ChuyenBay* cb = timChuyenBay(cayChuyenBay, maChuyenBay);
    if (cb == NULL) {
        setColor(12);
        cout << "Khong tim thay chuyen bay!\n";
        setColor(7);
        return;
    }

    setColor(11);
    cout << "Chuyen bay: " << cb->maChuyenBay << " - " << cb->sanBayDen << endl;
    cout << string(60, '-') << endl;
    setColor(7);

    NodeGhe* ghe = cb->danhSachGheTrong;
    if (ghe == NULL) {
        cout << "Khong con ghe trong!\n";
    } else {
        cout << "Cac ghe con trong: ";
        vector<int> arr;
        while (ghe != NULL) {
            arr.push_back(ghe->soGhe);
            ghe = ghe->next;
        }
        sort(arr.begin(), arr.end());
        for (size_t i = 0; i < arr.size(); ++i) {
            cout << arr[i];
            if (i + 1 < arr.size()) cout << ", ";
        }
        cout << endl;
    }

    cout << "\n";
    hienThiKhachHangTheoChuyenBay(cb);
}

void demChuyenBayRecursive(ChuyenBay* root, string soHieuMayBay, int& count) {
    if (root == NULL) return;

    if (root->soHieuMayBay == soHieuMayBay && root->trangThai == 3) {
        count++;
    }

    demChuyenBayRecursive(root->left, soHieuMayBay, count);
    demChuyenBayRecursive(root->right, soHieuMayBay, count);
}

void thongKeSoChuyenBayMayBay() {
    clearScreen();
    setColor(14);
    cout << "\n========== THONG KE CHUYEN BAY ==========\n\n";
    setColor(7);

    string soHieuMayBay;
    cout << "Nhap so hieu may bay: ";
    getline(cin, soHieuMayBay);
    soHieuMayBay = trim(soHieuMayBay);

    MayBay* mb = timMayBay(danhSachMayBay, soHieuMayBay);
    if (mb == NULL) {
        setColor(12);
        cout << "Khong tim thay may bay!\n";
        setColor(7);
        return;
    }

    int count = 0;
    demChuyenBayRecursive(cayChuyenBay, soHieuMayBay, count);

    setColor(11);
    cout << "May bay: " << mb->soHieuMayBay << endl;
    cout << "So cho: " << mb->soCho << endl;
    cout << "So chuyen bay da hoan tat: " << count << endl;
    setColor(7);
}

void menuThongKe() {
    while (true) {
        clearScreen();
        setColor(14);
        cout << "\n========== MENU THONG KE ==========\n";
        setColor(7);
        cout << "1. Hien thi danh sach khach hang theo chuyen bay\n";
        cout << "2. Hien thi danh sach ghe trong theo chuyen bay\n";
        cout << "3. Thong ke so chuyen bay cua may bay\n";
        cout << "0. Quay lai\n";
        setColor(11);
        cout << "Lua chon: ";
        setColor(7);

        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: {
                clearScreen();
                string ma;
                cout << "Nhap ma chuyen bay: ";
                getline(cin, ma);
                ChuyenBay* cb = timChuyenBay(cayChuyenBay, trim(ma));
                hienThiKhachHangTheoChuyenBay(cb);
                break;
            }
            case 2:
                thongKeGheTrong();
                break;
            case 3:
                thongKeSoChuyenBayMayBay();
                break;
            case 0:
                return;
            default:
                setColor(12);
                cout << "Lua chon khong hop le!\n";
                setColor(7);
                Sleep(1000);
                continue;
        }

        cout << "\nNhan phim bat ky de tiep tuc...";
        _getch();
    }
}

void menuQuanLy() {
    if (!dangNhap()) {
        return;
    }

    while (true) {
        clearScreen();
        setColor(14);
        cout << "\n========== MENU QUAN LY ==========\n";
        setColor(7);
        cout << "1. Xu ly dat ve\n";
        cout << "2. Xu ly tra ve\n";
        cout << "3. Thong ke\n";
        cout << "0. Dang xuat\n";
        setColor(11);
        cout << "Lua chon: ";
        setColor(7);

        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                xuLyDatVe();
                break;
            case 2:
                xuLyTraVe();
                break;
            case 3:
                menuThongKe();
                break;
            case 0:
                return;
            default:
                setColor(12);
                cout << "Lua chon khong hop le!\n";
                setColor(7);
                Sleep(1000);
                continue;
        }

        cout << "\nNhan phim bat ky de tiep tuc...";
        _getch();
    }
}

void menuChinh() {
    while (true) {
        clearScreen();
        setColor(14);
        cout << "\n";
        cout << "╔═══════════════════════════════════════════╗\n";
        cout << "║   HE THONG QUAN LY CHUYEN BAY NOI DIA    ║\n";
        cout << "╚═══════════════════════════════════════════╝\n";
        setColor(7);
        cout << "\n1. Hien thi danh sach chuyen bay\n";
        cout << "2. Dat ve\n";
        cout << "3. Quan ly (Danh cho nhan vien)\n";
        cout << "0. Thoat\n";
        setColor(11);
        cout << "\nLua chon: ";
        setColor(7);

        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                hienThiDanhSachChuyenBay();
                cout << "Nhan phim bat ky de tiep tuc...";
                _getch();
                break;
            case 2:
                datVe();
                cout << "\nNhan phim bat ky de tiep tuc...";
                _getch();
                break;
            case 3:
                menuQuanLy();
                break;
            case 0:
                setColor(10);
                cout << "\nCam on ban da su dung he thong!\n";
                setColor(7);
                return;
            default:
                setColor(12);
                cout << "Lua chon khong hop le!\n";
                setColor(7);
                Sleep(1000);
        }
    }
}


int main() {
    SetConsoleOutputCP(CP_UTF8);

    docFileMayBay();
    docFileChuyenBay();
    docFileKhachHang();

    if (danhSachMayBay == NULL) {
        setColor(12);
        cout << "  Canh bao: Khong co du lieu may bay. Vui long tao file MayBay.txt\n";
        cout << "   Vi du: VN123,180\n";
        setColor(7);
    }

    if (cayChuyenBay == NULL) {
        setColor(12);
        cout << "  Canh bao: Khong co du lieu chuyen bay. Vui long tao file ChuyenBay.txt\n";
        cout << "   Vi du: CB001,VN123,01/01/2024,Ha Noi,1\n";
        setColor(7);
    }

    ifstream checkAdmin("Admin.txt");
    if (!checkAdmin.is_open()) {
        ofstream createAdmin("Admin.txt");
        createAdmin << "admin,admin123\n";
        createAdmin << "user1,pass123\n";
        createAdmin.close();
        setColor(10);
        cout << "Da tao file Admin.txt mac dinh (user: admin, pass: admin123)\n";
        setColor(7);
        Sleep(1500);
    } else {
        checkAdmin.close();
    }

    menuChinh();

    setColor(10);
    cout << "\nDang luu du lieu...\n";
    ghiFileMayBay();
    ghiFileChuyenBay();
    ghiFileKhachHang();
    cout << "Hoan tat!\n";
    setColor(7);

    return 0;
}
 
