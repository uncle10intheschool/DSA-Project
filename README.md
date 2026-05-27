# 🚀 ĐỒ ÁN: SORTING BENCHMARK

**THÔNG TIN SINH VIÊN**
* **Môn học:** Cấu trúc dữ liệu và giải thuật
* **Học kỳ:** Học kỳ 2 - Năm học 2025-2026
* **Sinh viên:** [Tên để đây]
* **MSSV:** [Chờ bạn cung cấp]

---

## GIỚI THIỆU

Dự án **Sorting Benchmark** là một nghiên cứu thực chiến về tối ưu hóa hiệu năng thuật toán sắp xếp. Mục tiêu cốt lõi của đồ án không chỉ dừng ở tính đúng đắn của logic sắp xếp, mà là giải quyết bài toán về tài nguyên hệ thống, vượt qua trạng thái Time Limit Exceeded (TLE) khi đối mặt với dữ liệu quy mô lớn.

Thay vì tiếp cận theo hướng sử dụng các hàm tiêu chuẩn, dự án áp dụng quy trình với 4 bước nghiêm ngặt:
1. **Phân tích:** Lựa chọn, thiết kế thuật toán lõi dựa trên từng loại bài đặc trưng có phân bố dữ liệu khác nhau.
2. **Tấn công:** Khai thác các điểm mù vật lý của thuật toán thông qua kỹ thuật kiểm thử đối kháng để tạo ra các test case nhằm ép các giải thuật khác suy thoái hiệu năng về trạng thái tệ nhất.
3. **Tối ưu:** Can thiệp sâu ở mức độ bộ nhớ thô và luồng I/O để tái cấu trúc hệ thống phòng thủ và tối ưu tốc độ thuật toán.
4. **Đánh giá:** Đo lường hiệu quả giữa kiến trúc nền tảng và kiến trúc tối ưu.

---

## CẤU TRÚC THƯ MỤC

```text
Sorting-Benchmark
 ┣ README.md         # Báo cáo kỹ thuật tổng thể
 ┣ test_gen.cpp      # Bộ sinh Test cases
 ┣ Benchmark1        # Phiên bản kiến trúc nền tảng
 ┃ ┣  a1.cpp           # Bài A: Integer Sort 
 ┃ ┣  b1.cpp           # Bài B: Lexicographic Sort 
 ┃ ┗  c1.cpp           # Bài C: Length-aware Lexicographic Sort 
 ┗ Benchmark2        # Phiên bản tối ưu
   ┣  a2.cpp           # Bài A: Tối ưu Integer Sort
   ┣  b2.cpp           # Bài B: Tối ưu Lexicographic Sort
   ┗  c2.cpp           # Bài C: Tối ưu Length-aware Lexicographic Sort
```

---

## PHẦN 1 — BENCHMARK 1

Ở lần chạy đầu tiên, chiến lược cốt lõi là từ bỏ các cấu trúc so sánh phần tử tổng quát, tập trung thiết kế các giải thuật phân hoạch và phân phối chuyên biệt hóa theo từng đặc trưng của dữ liệu nhằm bứt phá khỏi giới hạn hiệu năng của các thuật toán sắp xếp thông thường: O(NlogN).

---

### BÀI A — Integer Sort

* **Thuật toán sử dụng:** Radix Sort cơ số 256 (Dịch bit và phân rã tuần tự từ phải sang trái - LSD).
* **Độ phức tạp:** Thời gian: O(4 × N) ≈ O(N), Không gian: O(N).

#### Cơ chế

Radix Sort phân rã số nguyên 32-bit thành 4 byte, thực hiện 4 vòng Counting Sort ổn định từ byte thấp đến byte cao. Số nguyên âm được xử lý branchless bằng XOR với `0x80000000` khi nạp — chuyển về dải không dấu nhưng giữ nguyên thứ tự, rồi XOR ngược lại khi xuất kết quả.

```cpp
// Minh họa cơ chế biến đổi không rẽ nhánh (branchless) khi xử lý số có dấu
a[i] = (ui)x ^ 0x80000000;
```

#### Lý do chọn

Thuật toán so sánh bị chặn ở O(N log N). Radix Sort cơ số 256 cho O(N × d) với d = 4 cố định → O(4N) tuyến tính thực sự. Loại bỏ hoàn toàn `if-else` giúp CPU tối ưu bộ dự đoán nhánh lệnh, tăng throughput tối đa trên mảng số nguyên lớn.

---

### BÀI B — Lexicographic Sort

* **Thuật toán sử dụng:** Multi-Key QuickSort (3-way partitioning) chạy trực tiếp trên đối tượng `std::string`.
* **Độ phức tạp:** Thời gian: Trung bình O(N log N + L) (với L là tổng chiều dài các tiền tố chung).

#### Cơ chế

MKQS chia mảng chuỗi thành 3 nhóm theo ký tự Pivot tại độ sâu `charIdx`: nhỏ hơn, bằng, lớn hơn. Chỉ nhóm "bằng Pivot" tăng `charIdx + 1` ở bước đệ quy tiếp — hai nhóm còn lại giữ nguyên độ sâu. Nhóm dưới 13 phần tử tự động chuyển sang Insertion Sort.

```cpp
// Minh họa tiến trình đệ quy 3 hướng, cô lập và bỏ qua các tiền tố đã khớp
customQuickSort(a, start, lower - 1, charIdx);
if (pivotChar >= 0) customQuickSort(a, lower, higher, charIdx + 1);
customQuickSort(a, higher + 1, end, charIdx);
```

#### Lý do chọn

Thuật toán thông thường phải quét lại từ đầu mỗi chuỗi ở mỗi bước phân hoạch — lãng phí khi dữ liệu có nhiều tiền tố chung dài. MKQS khắc phục bằng cơ chế khóa tiền tố: nhóm "bằng Pivot" đã khớp phần đầu, hệ thống chỉ xét tiếp `charIdx + 1` và không bao giờ duyệt lại.

---

### BÀI C — Length-aware String Sort

* **Thuật toán sử dụng:** Phân loại theo chiều dài (Bucket Sort) kết hợp với Radix Sort chuỗi ký tự.
* **Độ phức tạp:** Thời gian: O(N + Σ(Bucket\_len × len)), Không gian: O(N).

#### Cơ chế

101 Bucket tĩnh tương ứng chiều dài 1–100. Một vòng quét tuyến tính phân phối từng chuỗi vào đúng bucket qua `emplace_back`. Mỗi bucket sau đó chạy Radix Sort độc lập từ ký tự cuối (`len-1`) về đầu (`0`).

```cpp
// Minh họa bước tiền xử lý phân phối chuỗi vào các Bucket theo chiều dài tuyệt đối
for (int i = 0; i < n; i++) {
    string s; cin >> s;
    buckets[s.size()].emplace_back(s);
}
```

#### Lý do chọn

Lồng điều kiện độ dài vào hàm so sánh thông thường tạo rẽ nhánh ở mọi cặp phần tử. Bucket Sort triệt tiêu điều đó hoàn toàn: mọi chuỗi trong cùng bucket đã có độ dài bằng nhau, Radix Sort chạy thuần tuyến tính O(N × len) mà không cần kiểm tra thêm.

---

## PHẦN 2 — TEST CASES

`test_gen.cpp` sinh dữ liệu đối kháng tự động. Mỗi bài có 5 Test Case chuyên biệt ép thuật toán rơi vào Worst-case gây TLE hoặc OOM.

---

### BÀI A — Integer Sort (`generate_int_test`)

Nhắm phá vỡ QuickSort pivot tĩnh và bẻ gãy bộ nhớ Counting/Bucket Sort dựa trên biên độ Max-Min. Với N = 100000:

* **Test 1 (Giảm dần từ INT_MAX):** Dãy nghịch thế cực đoan, ép phân hoạch QuickSort lệch tối đa, đẩy đệ quy lên O(N) và gây Stack Overflow.

* **Test 2 (Tăng dần từ INT_MIN):** Mảng đã sắp xếp với số âm cực tiểu. Kiểm tra lỗi rẽ nhánh số âm/dương.

* **Test 3 (Xen kẽ INT_MIN, INT_MAX, 0 và ngẫu nhiên):** Biên độ Max-Min ~4 tỷ. Counting/Bucket Sort cấp phát mảng kích thước Max-Min sẽ lập tức OOM.

* **Test 4 (Ngẫu nhiên toàn cục):** Khảo sát hiệu năng tổng quát và tốc độ luồng I/O tiêu chuẩn.

* **Test 5 (99% phần tử trùng lặp):** 99.000 phần tử trùng + 1.000 ngẫu nhiên. Vô hiệu cơ chế chia QuickSort 2 hướng, ép tràn stack do phân hoạch lệch hoàn toàn.

---

### BÀI B — Lexicographic Sort (`generate_strlexi_test`)

Nhắm vào trượt cache và chi phí so sánh chuỗi nguyên khối. Với N = 100000:

* **Test 1 & Test 3 (Bẫy tiền tố sâu + Đuôi đảo chiều):** Chuỗi 100 ký tự, 98 đầu toàn `'a'`, 2 cuối dao động (`z-a`, `a-z`). Ép `operator<` duyệt 98 ký tự vô nghĩa hàng triệu lần, gây trượt cache và cạn kiệt chu kỳ CPU.

* **Test 2 (Dãy nghịch thế đồng nhất):** Chuỗi 100 ký tự bắt đầu `'z'` giảm dần. Đánh gục `swap(string, string)` — C++ liên tục cấp phát/hủy heap cho chuỗi dài.

* **Test 4 & Test 5 (Chiều dài và ký tự ngẫu nhiên):** Chuỗi dài 10–100 ký tự ngẫu nhiên. Đánh giá I/O và chi phí cấp phát `string` khi kích thước biến thiên liên tục.

---

### BÀI C — Length-aware String Sort (`generate_strlenlexi_test`)

Phá hủy bước tiền xử lý Bucket Sort. Với N = 10.000:

* **Test 1 & Test 4 (Phá hủy Bucket + Nghịch thế sâu):** Toàn bộ 100 ký tự, tiền tố 96 chữ `'a'`/`'b'`, 4 cuối nghịch thế. Dồn hết vào bucket 100, vô hiệu tiền xử lý — nghẽn so sánh tiền tố sâu.

* **Test 2 (Sụp đổ phân phối):** 10.000 chuỗi đồng nhất 100 ký tự `'z'`. Gây nghẽn đệ quy nếu thuật toán lõi không có cơ chế dừng sớm.

* **Test 3 (Bẫy xen kẽ chẵn lẻ):** 10.000 chuỗi 100 ký tự, 99 chữ `'a'` + đuôi xen kẽ `'x'`/`'y'`. Ép phân hoạch đến ký tự cuối liên tục, đo chi phí đệ quy sâu.

* **Test 5 (Ngẫu nhiên khóa độ dài):** Chuỗi ngẫu nhiên, chiều dài cố định 100. Vô hiệu Bucket Sort, ép thuật toán lõi xử lý hoán vị chuỗi ở cường độ cao nhất.

---

## PHẦN 3 — BENCHMARK 2

Sau khi phân tích các nút thắt ở Benchmark 1 và bị tấn công bằng test đối kháng, phiên bản Benchmark 2 can thiệp sâu vào luồng I/O, Layout bộ nhớ và đổi mới tư duy thuật toán để thiết lập lớp phòng thủ.

---

### BÀI A — Tối ưu Integer Sort

**Điểm yếu P1:** Sao chép dữ liệu liên tục giữa mảng chính và mảng phụ qua 4 vòng cố định gây nút thắt băng thông RAM. `cin`/`cout` quá chậm trước dữ liệu lớn.

#### Tối ưu 1 — Nâng cấp cơ số phân rã

Tăng cơ số mảng đếm tần số từ 256 lên 65536 (16-bit). Việc này ép thuật toán chỉ cần lặp đúng 2 lần (lần 1 xét 16 bit thấp, lần 2 xét 16 bit cao), cắt giảm chính xác 50% thời gian xử lý và thao tác copy dữ liệu trên RAM.

```cpp
// Vòng lặp tối ưu nhảy bước 16-bit, giảm số vòng lặp từ 4 xuống 2
for (int shift = 0; shift < 32; shift += 16) {
    for (int i = 0; i < 65536; i++) countFreq[i] = 0;
    // Phân phối bucket...
}
```

#### Tối ưu 2 — Custom Fast I/O

Loại bỏ iostream. Tự viết hàm giải mã từng byte ký tự `cin.get()` (hàm `read`) và bộ đệm số `cout.put()` (hàm `write`), tốc độ đọc/ghi vươn đến giới hạn vật lý của ổ đĩa cứng.

```cpp
// Kỹ thuật dịch số nguyên trực tiếp từ mã ASCII
while (c >= '0' && c <= '9') {
    x = x * 10 + (c - '0');
    c = cin.get();
}
```

---

### BÀI B — Tối ưu Lexicographic Sort

**Điểm yếu P1:** `swap` trực tiếp `std::string` tốn kém do cấp phát/hủy heap liên tục. Pivot tĩnh (phần tử đầu) dễ suy thoái O(N²) khi gặp test đối kháng.

#### Tối ưu 1 — Khai tử đối tượng, dùng bộ đệm tĩnh

Hệ thống cấp phát một mảng tĩnh khổng lồ `char bufferIn[11000001]`. Thay vì đọc từng chuỗi, sử dụng duy nhất 1 lệnh `fread` để ném thẳng cục dữ liệu từ file vào RAM trong chớp mắt.

```cpp
// Đọc toàn bộ đầu vào trong 1 dòng lệnh siêu tốc
size_t inputLen = fread(bufferIn, 1, MAX_BUFFER_SIZE, stdin);
bufferIn[inputLen] = '\0';
```

#### Tối ưu 2 — Sắp xếp gián tiếp qua Con trỏ

Thuật toán lưu lại các ranh giới của chuỗi vào mảng con trỏ thô `char* strPointers[100001]`. Ở quá trình phân hoạch MKQS, CPU chỉ tiến hành swap các địa chỉ ô nhớ (chỉ mất 8 bytes) thay vì bê vác hàng trăm ký tự.

```cpp
// So sánh và hoán đổi trực tiếp địa chỉ bộ nhớ
if (getCharAtDepth(strPointers[mid], depth) < getCharAtDepth(strPointers[left], depth))
    swap(strPointers[left], strPointers[mid]);
```

---

### BÀI C — Tối ưu Length-aware Sort

**Điểm yếu P1:** `vector` động gây phân mảnh heap. Sụp đổ hoàn toàn khi toàn bộ dữ liệu cùng chiều dài, dồn vào 1 Bucket duy nhất.

**Lý do đổi thuật toán:** Bucket Sort không sinh tồn được trước phân bố đồng nhất. Benchmark 2 chuyển sang **IntroSort** — thuật toán lai đảm bảo O(N log N) trong mọi trường hợp.

#### Tối ưu 1 — Kiến trúc IntroSort Đa tầng

Hệ thống chạy QuickSort làm nền. Nếu phát hiện bộ test cố tình gài bẫy đẩy độ sâu đệ quy vượt mức giới hạn `2 * log2(n)`, nó lập tức "bẻ lái" sang HeapSort để giữ vững hiệu năng. Với mảng nhỏ hơn 16 phần tử, tự động dùng InsertionSort.

```cpp
// Cơ chế phòng ngự của IntroSort
if (depthLimit == 0) { // Bị ép đệ quy quá sâu
    HeapSort(idx, left, right, a); // Lập tức bọc lót bằng HeapSort O(N log N)
    return;
}
```

#### Tối ưu 2 — Indirect Sorting

Cũng để né việc `swap` các đối tượng `string`, bài C sử dụng mảng số nguyên `vector<int> idx`. Các chuỗi gốc nằm yên vị trí trên RAM để bảo toàn cấu trúc tuần tự của CPU Cache, hệ thống chỉ sắp xếp các con số nguyên nội bộ bên trong `idx`.

```cpp
// Hàm so sánh kiểm tra độ dài trước, duyệt từ điển sau thông qua chỉ số
bool inline lengthAwareLess(const string& a, const string& b) {
    if (a.length() != b.length()) return a.length() < b.length();
    return a < b;
}
// Khi đổi chỗ, chỉ đổi chỗ các con số Integer
swap(idx[left], idx[mid]);
```

---

## PHẦN 4 — ĐÁNH GIÁ

### Bảng so sánh

| Tiêu chí | Benchmark 1 | Benchmark 2 |
|---|---|---|
| **I/O** | Nghẽn tại `cin`/`cout` | Custom Fast I/O / `fread` |
| **RAM** | Phân mảnh heap động | Zero Allocation — bộ đệm tĩnh |
| **Swap** | Di chuyển byte chuỗi thô | Pointer Indexing — 8 bytes |
| **Test đối kháng** | Dễ TLE / OOM | Miễn nhiễm — kiến trúc lai |

### Bài học cốt lõi

| # | Insight |
|---|---|
| 1 | **Big-O không quyết định tất cả** — tối ưu Cache Locality và Branchless vượt trội hơn lý thuyết thuần túy |
| 2 | **Chi phí ẩn từ thư viện** — `std::string`, `vector` chậm ở scale lớn; bộ đệm tĩnh và con trỏ thô là chìa khóa |
| 3 | **Tư duy phòng thủ** — test đối kháng phát hiện sớm rủi ro, dẫn đến giải thuật lai an toàn (IntroSort) |
