#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <windows.h>

using namespace std;

struct MayBay {
    char soHieu[20];
    int soCho;
};

struct NodeGhe {
    int soGhe;
    NodeGhe* next;
};

struct ChuyenBay {
    char maChuyenBay[20];
    char soHieuMayBay[20];
    char ngayKhoiHanh[15];
    char sanBayDen[50];
    int trangThai;
    NodeGhe* danhSachGheTrong;
};

struct NodeMayBay {
    MayBay data;
    NodeMayBay* next;
};

struct NodeChuyenBay {
    ChuyenBay data;
    NodeChuyenBay* next;
};

NodeGhe* khoiTaoDanhSachGhe(int soCho) {
    if (soCho <= 0) return NULL;
    NodeGhe* head = new NodeGhe;
    head->soGhe = 1;
    head->next = NULL;
    NodeGhe* current = head;
    for (int i = 2; i <= soCho; i++) {
        NodeGhe* newNode = new NodeGhe;
        newNode->soGhe = i;
        newNode->next = NULL;
        current->next = newNode;
        current = newNode;
    }
    return head;
}

void themGheTrong(NodeGhe*& head, int soGhe) {
    NodeGhe* newNode = new NodeGhe;
    newNode->soGhe = soGhe;
    newNode->next = NULL;
    if (head == NULL || head->soGhe > soGhe) {
        newNode->next = head;
        head = newNode;
        return;
    }
    NodeGhe* current = head;
    while (current->next != NULL && current->next->soGhe < soGhe) {
        current = current->next;
    }
    newNode->next = current->next;
    current->next = newNode;
}

bool xoaGheTrong(NodeGhe*& head, int soGhe) {
    if (head == NULL) return false;
    if (head->soGhe == soGhe) {
        NodeGhe* temp = head;
        head = head->next;
        delete temp;
        return true;
    }
    NodeGhe* current = head;
    while (current->next != NULL) {
        if (current->next->soGhe == soGhe) {
            NodeGhe* temp = current->next;
            current->next = temp->next;
            delete temp;
            return true;
        }
        current = current->next;
    }
    return false;
}

int demGheTrong(NodeGhe* head) {
    int count = 0;
    NodeGhe* current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

void giaiPhongDanhSachGhe(NodeGhe*& head) {
    while (head != NULL) {
        NodeGhe* temp = head;
        head = head->next;
        delete temp;
    }
}

void themMayBay(NodeMayBay*& head, MayBay mb) {
    NodeMayBay* newNode = new NodeMayBay;
    newNode->data = mb;
    newNode->next = NULL;
    if (head == NULL) {
        head = newNode;
    }
    else {
        NodeMayBay* temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

MayBay* timMayBay(NodeMayBay* head, const char* soHieu) {
    NodeMayBay* current = head;
    while (current != NULL) {
        if (strcmp(current->data.soHieu, soHieu) == 0) {
            return &(current->data);
        }
        current = current->next;
    }
    return NULL;
}

bool kiemTraMayBayTonTai(NodeMayBay* head, const char* soHieu) {
    return timMayBay(head, soHieu) != NULL;
}

void docFileMayBay(NodeMayBay*& head, const char* filename = "MayBay.txt") {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Khong the mo file " << filename << endl;
        return;
    }
    MayBay mb;
    while (file >> mb.soHieu >> mb.soCho) {
        themMayBay(head, mb);
    }
    file.close();
}

void ghiFileMayBay(NodeMayBay* head, const char* filename = "MayBay.txt") {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Khong the mo file " << filename << endl;
        return;
    }
    NodeMayBay* current = head;
    while (current != NULL) {
        file << current->data.soHieu << " " << current->data.soCho << endl;
        current = current->next;
    }
    file.close();
}

void hienThiDanhSachMayBay(NodeMayBay* head) {
    if (head == NULL) {
        cout << "Danh sach may bay trong!" << endl;
        return;
    }
    cout << "\n*********** DANH SACH MAY BAY *************" << endl;
    cout << "So hieu\t\tSo cho" << endl;
    cout << "---------------------------------------" << endl;
    NodeMayBay* current = head;
    while (current != NULL) {
        cout << current->data.soHieu << "\t\t" << current->data.soCho << endl;
        current = current->next;
    }
    cout << "******************************************" << endl;
}

void giaiPhongDanhSachMayBay(NodeMayBay*& head) {
    while (head != NULL) {
        NodeMayBay* temp = head;
        head = head->next;
        delete temp;
    }
}

void themChuyenBay(NodeChuyenBay*& head, ChuyenBay cb) {
    NodeChuyenBay* newNode = new NodeChuyenBay;
    newNode->data = cb;
    newNode->next = NULL;
    if (head == NULL) {
        head = newNode;
    }
    else {
        NodeChuyenBay* temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

ChuyenBay* timChuyenBay(NodeChuyenBay* head, const char* maChuyenBay) {
    NodeChuyenBay* current = head;
    while (current != NULL) {
        if (strcmp(current->data.maChuyenBay, maChuyenBay) == 0) {
            return &(current->data);
        }
        current = current->next;
    }
    return NULL;
}

void docFileChuyenBay(NodeChuyenBay*& head, NodeMayBay* dsMayBay, const char* filename = "ChuyenBay.txt") {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Khong the mo file " << filename << endl;
        return;
    }
    ChuyenBay cb;
    while (file >> cb.maChuyenBay >> cb.soHieuMayBay >> cb.ngayKhoiHanh) {
        file.ignore();
        file.getline(cb.sanBayDen, 50);
        file >> cb.trangThai;
        MayBay* mb = timMayBay(dsMayBay, cb.soHieuMayBay);
        if (mb != NULL) {
            cb.danhSachGheTrong = khoiTaoDanhSachGhe(mb->soCho);
        }
        else {
            cb.danhSachGheTrong = NULL;
        }
        themChuyenBay(head, cb);
    }
    file.close();
}

void ghiFileChuyenBay(NodeChuyenBay* head, const char* filename = "ChuyenBay.txt") {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Khong the mo file " << filename << endl;
        return;
    }
    NodeChuyenBay* current = head;
    while (current != NULL) {
        file << current->data.maChuyenBay << " "
            << current->data.soHieuMayBay << " "
            << current->data.ngayKhoiHanh << " "
            << current->data.sanBayDen << endl
            << current->data.trangThai << endl;
        current = current->next;
    }
    file.close();
}

const char* layTenTrangThai(int trangThai) {
    switch (trangThai) {
    case 0: return "Huy chuyen";
    case 1: return "Con ve";
    case 2: return "Het ve";
    case 3: return "Hoan tat";
    default: return "Khong xac dinh";
    }
}

void hienThiDanhSachChuyenBay(NodeChuyenBay* head) {
    if (head == NULL) {
        cout << "Danh sach chuyen bay trong!" << endl;
        return;
    }
    cout << "\n===================== DANH SACH CHUYEN BAY =====================" << endl;
    cout << "Ma CB\t\tMay bay\t\tNgay KH\t\tSan bay den\t\tTrang thai\tGhe trong" << endl;
    cout << "================================================================" << endl;
    NodeChuyenBay* current = head;
    while (current != NULL) {
        int soGheTrong = demGheTrong(current->data.danhSachGheTrong);
        cout << current->data.maChuyenBay << "\t\t"
            << current->data.soHieuMayBay << "\t\t"
            << current->data.ngayKhoiHanh << "\t"
            << current->data.sanBayDen << "\t\t"
            << layTenTrangThai(current->data.trangThai) << "\t\t"
            << soGheTrong << endl;
        current = current->next;
    }
    cout << "================================================================" << endl;
}

void capNhatTrangThaiChuyenBay(ChuyenBay* cb) {
    if (cb == NULL) return;
    if (cb->trangThai == 0 || cb->trangThai == 3) {
        return;
    }
    int soGheTrong = demGheTrong(cb->danhSachGheTrong);
    if (soGheTrong == 0) {
        cb->trangThai = 2;
    }
    else {
        cb->trangThai = 1;
    }
}

void giaiPhongDanhSachChuyenBay(NodeChuyenBay*& head) {
    while (head != NULL) {
        giaiPhongDanhSachGhe(head->data.danhSachGheTrong);
        NodeChuyenBay* temp = head;
        head = head->next;
        delete temp;
    }
}

void xoaManHinh() {
    system("cls");
}

void dungManHinh() {
    cout << "\nNhan ENTER de tiep tuc...";
    cin.get();
}

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

int main() {
    NodeMayBay* dsMayBay = NULL;
    NodeChuyenBay* dsChuyenBay = NULL;
    docFileMayBay(dsMayBay);
    docFileChuyenBay(dsChuyenBay, dsMayBay);
    int luaChon;
    do {
        xoaManHinh();
        setColor(14);
        cout << "\n************ QUAN LY MAY BAY & CHUYEN BAY ***************" << endl;
        setColor(14);
        cout << "*             1. Hien thi danh sach may bay             *" << endl;
        cout << "*             2. Hien thi danh sach chuyen bay          *" << endl;
        cout << "*             3. Them may bay moi                       *" << endl;
        cout << "*             4. Them chuyen bay moi                    *" << endl;
        cout << "*             0. Thoat va luu du lieu                   *" << endl;
        cout << "*********************************************************" << endl;
        setColor(7);
        cout << "Nhap lua chon: ";
        cin >> luaChon;
        cin.ignore();
        switch (luaChon) {
        case 1:
            xoaManHinh();
            hienThiDanhSachMayBay(dsMayBay);
            dungManHinh();
            break;
        case 2:
            xoaManHinh();
            hienThiDanhSachChuyenBay(dsChuyenBay);
            dungManHinh();
            break;
        case 3: {
            xoaManHinh();
            MayBay mb;
            cout << "Nhap so hieu may bay: ";
            cin >> mb.soHieu;
            if (kiemTraMayBayTonTai(dsMayBay, mb.soHieu)) {
                setColor(12);
                cout << "So hieu may bay da ton tai!" << endl;
                setColor(7);
            }
            else {
                cout << "Nhap so cho: ";
                cin >> mb.soCho;
                if (mb.soCho > 0) {
                    themMayBay(dsMayBay, mb);
                    setColor(10);
                    cout << "Them may bay thanh cong!" << endl;
                    setColor(7);
                }
                else {
                    setColor(12);
                    cout << "So cho phai lon hon 0!" << endl;
                    setColor(7);
                }
            }
            dungManHinh();
            break;
        }
        case 4: {
            xoaManHinh();
            ChuyenBay cb;
            cout << "Nhap ma chuyen bay: ";
            cin >> cb.maChuyenBay;
            if (timChuyenBay(dsChuyenBay, cb.maChuyenBay) != NULL) {
                setColor(12);
                cout << "Ma chuyen bay da ton tai!" << endl;
                setColor(7);
            }
            else {
                cout << "Nhap so hieu may bay: ";
                cin >> cb.soHieuMayBay;
                MayBay* mb = timMayBay(dsMayBay, cb.soHieuMayBay);
                if (mb == NULL) {
                    setColor(12);
                    cout << "So hieu may bay khong ton tai!" << endl;
                    setColor(7);
                }
                else {
                    cout << "Nhap ngay khoi hanh (dd/mm/yyyy): ";
                    cin >> cb.ngayKhoiHanh;
                    cin.ignore();
                    cout << "Nhap san bay den: ";
                    cin.getline(cb.sanBayDen, 50);
                    cb.trangThai = 1;
                    cb.danhSachGheTrong = khoiTaoDanhSachGhe(mb->soCho);
                    themChuyenBay(dsChuyenBay, cb);
                    setColor(10);
                    cout << "Them chuyen bay thanh cong!" << endl;
                    setColor(7);
                }
            }
            dungManHinh();
            break;
        }
        case 0:
            ghiFileMayBay(dsMayBay);
            ghiFileChuyenBay(dsChuyenBay);
            setColor(10);
            cout << "Da luu du lieu. Tam biet!" << endl;
            setColor(7);
            break;
        default:
            setColor(12);
            cout << "Lua chon khong hop le!" << endl;
            setColor(7);
            dungManHinh();  
        }
    } 
    
    while (luaChon != 0);
    giaiPhongDanhSachMayBay(dsMayBay);
    giaiPhongDanhSachChuyenBay(dsChuyenBay);
    return 0;
}