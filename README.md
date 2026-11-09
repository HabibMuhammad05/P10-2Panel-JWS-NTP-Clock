# P10 2-Panel JWS NTP Clock

Jadwal Waktu Sholat (JWS) digital berbasis **ESP8266** dengan **2 panel LED P10**. Waktu diambil dari **server NTP lewat WiFi**, sehingga tidak memerlukan modul RTC DS3231 lagi.

Proyek ini adalah modifikasi dari [busel7/Arduino – MembuatJWS](https://github.com/busel7/Arduino/tree/master/MembuatJWS) (Proyek Untuk 1000 Masjid, ElektronMart). Pembaruan terakhir: 30 Januari 2025 oleh Habib M.

## Fitur

- Jadwal sholat 5 waktu + tanbih imsak
- Jam besar dan tanggal
- Alarm adzan dan tanbih imsak (buzzer)
- Hitung mundur iqomah per waktu sholat
- Pengaturan lewat browser (iqomah, durasi adzan, ihtiyati, koordinat, zona waktu, nama masjid, SSID/password WiFi)
- Update firmware lewat WiFi (OTA)
- Waktu otomatis dari NTP (`pool.ntp.org`)

## Perubahan dari versi aslinya

| Versi asli | Versi ini |
|---|---|
| Waktu dari RTC DS3231 | Waktu dari NTP (perangkat **wajib terhubung WiFi** saat boot) |
| Ubah jam manual lewat web | Tidak diperlukan, sinkron otomatis |
| WiFi aktif terus | WiFi dimatikan otomatis setelah **1 jam** menyala |

Setelah WiFi mati, jam tetap berjalan dari hitungan internal (`millis()`) dan disinkronkan ulang dari NTP setiap kali perangkat restart.

## Perangkat Keras

- NodeMCU ESP8266 / Wemos (Lolin) D1 Mini (clone)
- 2 panel LED P10 (disusun 2 kolom x 1 baris)
- Buzzer
- Power supply eksternal 5V untuk panel P10

### Wiring

| Pin P10 | GPIO | NodeMCU |
|---|---|---|
| A (2) | GPIO16 | D0 |
| B (4) | GPIO12 | D6 |
| CLK (8) | GPIO14 | D5 |
| SCK (10) | GPIO0 | D3 |
| R (12) | GPIO13 | D7 |
| NOE (1) | GPIO15 | D8 |
| GND (3) | GND | GND |

| Buzzer | NodeMCU |
|---|---|
| + | RX (GPIO3) |
| - | GND |

> **Catatan**
> - Panel P10 wajib memakai **power eksternal 5V**, jangan ditenagai dari pin ESP8266.
> - Saat upload program, **cabut sementara kabel buzzer** dari pin RX.

## Library yang Dibutuhkan

| Library | Sumber |
|---|---|
| DMDESP | https://github.com/busel7/DMDESP |
| PrayerTime | https://github.com/asmaklad/Arduino-Prayer-Times |
| ArduinoJson v6 | https://github.com/bblanchon/ArduinoJson |
| NTPClient | https://github.com/arduino-libraries/NTPClient |
| Time (TimeLib) | https://github.com/PaulStoffregen/Time |
| F1kM_Hisab *(opsional, kode tanggal Hijriah sedang dinonaktifkan)* | https://github.com/wardi1971/F1kM_Hisab |

Library bawaan board ESP8266: `ESP8266WiFi`, `ESP8266WebServer`, `ESP8266HTTPUpdateServer`, `LittleFS`.

Tools pendukung:
- Desain bitmap dan font: http://dotmatrixtool.com
- LittleFS Uploader: https://github.com/earlephilhower/arduino-esp8266littlefs-plugin/releases

## Struktur Proyek

```
P10-2Panel-JWS-NTP-Clock/
├── P10_2Panel_JWS_NTP_Clock.ino   # Program utama (setup, loop, state tampilan)
├── Display.h                      # Konfigurasi panel P10 dan tampilan
├── Buzzer.h                       # Pin dan fungsi buzzer
├── RTC.h                          # Waktu NTP -> TimeLib (nama file warisan dari versi RTC)
├── FSConfig.h                     # Baca/tulis config JSON, hitung jadwal, update waktu, XML data
├── JWS.h                          # Logika alarm, adzan, iqomah, dan tampilan sholat
├── WebServer.h                    # WiFi, web server, OTA, dan handler pengaturan
├── WebPage.h                      # Halaman web pengaturan
└── data/
    └── configjws.json             # Konfigurasi awal (diupload ke LittleFS)
```

## Cara Instalasi

1. Pasang board **ESP8266** di Arduino IDE, lalu install semua library di atas.
2. Buka `P10_2Panel_JWS_NTP_Clock.ino`, pilih board **LOLIN (WEMOS) D1 mini** atau **NodeMCU**.
3. Edit `data/configjws.json` dan **tambahkan SSID serta password WiFi** (lihat bagian Konfigurasi).
4. Cabut kabel buzzer dari pin RX.
5. Upload isi folder `data/` ke LittleFS memakai *LittleFS Uploader*.
6. Upload sketch ke board.
7. Pasang kembali kabel buzzer dan nyalakan dengan power eksternal 5V untuk panel.

Jika `configjws.json` belum ada di LittleFS, perangkat membuat file default sendiri lalu restart otomatis.

## Konfigurasi

Contoh `data/configjws.json`:

```json
{
  "iqmhs": "12",
  "iqmhd": "8",
  "iqmha": "6",
  "iqmhm": "5",
  "iqmhi": "5",
  "durasiadzan": "4",
  "ihti": "2",
  "latitude": "-6.16",
  "longitude": "106.61",
  "zonawaktu": "7",
  "namamasjid": "NAMA MASJID ANDA",
  "ssidData": "NAMA_WIFI",
  "passData": "PASSWORD_WIFI"
}
```

| Kunci | Keterangan |
|---|---|
| `iqmhs`, `iqmhd`, `iqmha`, `iqmhm`, `iqmhi` | Waktu iqomah (menit) untuk Subuh, Dzuhur, Ashar, Maghrib, Isya |
| `durasiadzan` | Durasi tampilan adzan (menit) |
| `ihti` | Koreksi ihtiyati jadwal sholat (menit) |
| `latitude`, `longitude` | Koordinat lokasi (contoh Indonesia: latitude bernilai negatif untuk belahan selatan) |
| `zonawaktu` | Zona waktu terhadap GMT (WIB = `7`, WITA = `8`, WIT = `9`) |
| `namamasjid` | Teks berjalan di layar |
| `ssidData`, `passData` | SSID dan password WiFi untuk mengambil waktu NTP |

> **Penting:** file `data/configjws.json` bawaan repo belum berisi `ssidData` dan `passData`. Tanpa keduanya perangkat tidak bisa terhubung ke WiFi. Isi sebelum upload, atau ubah lewat halaman web.
>
> Password WiFi tersimpan sebagai teks biasa di LittleFS, jadi jangan commit kredensial asli ke GitHub.

### Metode perhitungan jadwal sholat

Di `FSConfig.h` (fungsi `JadwalSholat()`):

- Metode: **Karachi**
- Mazhab Ashar: **Shafii**
- Sudut Subuh: **20°**, sudut Isya: **18°**
- Penyesuaian lintang tinggi: **AngleBased**

Ubah bagian ini bila ingin mengikuti metode lain, misalnya standar Kemenag.

## Cara Kerja Singkat

1. Saat boot, perangkat terhubung ke WiFi. Buzzer berbunyi berkala selama belum tersambung.
2. Waktu diambil dari `pool.ntp.org` lalu diterapkan ke TimeLib.
3. Bila epoch tidak valid (di luar rentang 1 Jan 2025 sampai batas atas), buzzer berbunyi dan perangkat **restart** untuk mencoba lagi.
4. Jadwal sholat dihitung dari koordinat, zona waktu, dan koreksi ihtiyati.
5. Setelah 1 jam WiFi dimatikan untuk menghemat daya dan mengurangi gangguan.

## Pengaturan lewat Browser

Pastikan perangkat dan HP/laptop berada di jaringan WiFi yang sama, lalu buka alamat IP perangkat (tampil di teks berjalan selama WiFi aktif).

| Endpoint | Fungsi |
|---|---|
| `/` | Halaman pengaturan |
| `/simpandatajws` (POST) | Simpan konfigurasi JSON |
| `/xmlwaktu` | Data waktu saat ini (XML) |
| `/xmldatajws` | Data konfigurasi JWS (XML) |
| `/update` | Update firmware OTA (`ESP8266HTTPUpdateServer`) |

Karena WiFi mati setelah 1 jam, pengaturan lewat browser hanya bisa dilakukan dalam 1 jam pertama setelah perangkat dinyalakan.

## Debugging

Aktifkan log serial dengan membuka komentar pada baris berikut di file `.ino`:

```cpp
//#define DEBUG
```

Baud rate: `115200`. Ingat bahwa pin RX dipakai buzzer, jadi cabut buzzer saat memakai Serial Monitor.

## Kredit

- Proyek asli: [busel7/Arduino](https://github.com/busel7/Arduino) – [www.grobak.net](https://www.grobak.net) – [www.elektronmart.com](https://www.elektronmart.com)
- Diskusi grup Untuk1000Masjid: https://www.facebook.com/groups/761058907424496/
- Modifikasi NTP Clock: Habib M. (2025)

## Lisensi

Belum ditentukan. Tambahkan file `LICENSE` sesuai kebutuhan, dan perhatikan lisensi proyek asli serta library yang dipakai.
