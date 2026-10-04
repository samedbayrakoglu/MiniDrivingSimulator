Mini Driving Simulator
README Taslağı

## 1. Mimari ve Teknik Kararlar

Proje, Unreal Engine 5.7 üzerinde Chaos Vehicles kullanılarak geliştirilmiştir.

### Temel Mimari

- Vehicle Pawn

- Chaos Vehicle Movement

- Input

- Telemetry Component

- UDP Telemetry Sender

- Event Component

### C++ / Blueprint Ayrımı

C++ tarafı sistem ve veri sorumlulukları için kullanılmıştır:

- Araç Pawn sınıfı

- Input yönetimi

- Telemetry data toplama

- UDP telemetry yayınlama

- Event logging

Blueprint tarafı ise Unreal'a özgü görsel ve asset yapılandırmaları için kullanılmıştır:

- Vehicle Blueprint

- Vehicle mesh ve component yapılandırması

- Kamera component'leri

- HUD / UMG

- Input Action ve Mapping Context asset'lerinin atanması

Bu ayrımın amacı, temel sistem davranışlarını C++ tarafında tutarken Unreal Editor üzerinden yapılması daha uygun olan görsel ve asset tabanlı yapılandırmaları Blueprint'te bırakmaktır.

### Chaos Vehicles

Araç fiziği sıfırdan geliştirilmemiş, Unreal Engine'in Chaos Vehicles sistemi kullanılmıştır. Bu tercih 48 saatlik süre içerisinde fizik modelini yeniden geliştirmek yerine araç kontrolü, telemetri ve iletişim mimarisine odaklanmayı sağlamıştır.

## 2. UDP Telemetri

Telemetri sistemi iki ayrı sorumluluğa ayrılmıştır:

- Telemetry Component Veriyi toplar ve güncel telemetry state'i tutar.

- UDP Sender Telemetry verisini serialize eder ve UDP üzerinden yayınlar.

### Veri Seti

- Sequence Number

- Speed

- RPM

- World Position (X, Y, Z)

- Steering

- Throttle

- Brake

### Format

Binary format tercih edilmiştir. Telemetry sabit frekansta ve çoğunlukla sayısal verilerden oluştuğu için binary format daha küçük paket boyutu ve düşük serialization overhead sağlamaktadır. Python receiver tarafında struct.unpack() kullanılarak paket decode edilmektedir.

- Paket yapısı: [uint32 Sequence Number] + [8 x float]

- Toplam paket boyutu: 36 byte

### Frekans

UDP telemetry yayın frekansı 100 Hz'dir.

Telemetry verisi araç hareketini güncel tutmak amacıyla her frame sample edilmektedir. UDP sender güncel telemetry state'ini 100 Hz frekansta yayınlamaktadır.

### Thread / Async

UDP socket non-blocking olarak oluşturulmuştur.

MVP kapsamında paketler küçük olduğu ve yayın frekansı 100 Hz ile sınırlı olduğu için ayrı bir worker thread kullanılmamıştır. Telemetry publishing game thread üzerinde gerçekleştirilmiştir.

Daha yüksek telemetry hacmi veya network yükü oluşması durumunda UDP sender ayrı bir worker thread'e taşınabilir.

### Paket Kaybı

UDP connectionless olduğu için paketlerin güvenilir şekilde teslim edilmesi garanti edilmemektedir.

Bu nedenle her pakete sequence number eklenmiştir. Python receiver beklenen sequence number ile gelen sequence number'ı karşılaştırarak paket kaybını tespit etmektedir.

Kayıp paketler yeniden gönderilmemektedir. Gerçek zamanlı telemetry için eski bir paketin sonradan tekrar gönderilmesindense güncel verinin alınması tercih edilmiştir.

## 3. Event Logging

Event Logging kapsamında iki event uygulanmıştır:

- Hard Braking

- Collision

Hard braking, fren veya handbrake input'u aktifken belirli bir negatif ivme eşiğinin aşılmasıyla tespit edilmektedir.

Collision event'leri Unreal'ın OnComponentHit callback'i üzerinden event-driven olarak alınmaktadır.

Collision event'i timestamp ve impact strength bilgisi ile loglanmaktadır. Aynı fiziksel çarpışmanın birden fazla hit callback oluşturabilmesi nedeniyle kısa bir aggregation window kullanılarak bu süre içerisindeki en yüksek impact strength tek event olarak kaydedilmektedir.

## 4. Unity Unreal Geçiş Analizi

Unity deneyiminden Unreal'a geçerken temel oyun motoru kavramlarının karşılıkları hızlı şekilde adapte edilmiştir.

Temel Karşılıklar

- Scene Level

- GameObject Actor

- Component Component

- Prefab Blueprint Class

- Inspector Details Panel

- Scene Hierarchy World Outliner

- Script C++ / Blueprint

- Input System Enhanced Input

Aktarılan Pratikler

- Component-based architecture

- Profiling / performance odaklı düşünme

- Input abstraction

- Gameplay sistemlerini sorumluluklarına ayırma

Özellikle telemetry sistemi Collector ve UDP Sender olarak ayrılarak tek bir component'in birden fazla sorumluluk üstlenmesinden kaçınılmıştır.

Karşılaşılan Farklılıklar

- Unreal'ın Chaos Vehicles ve Unreal-specific component mimarisi

- Unity'deki Rigidbody/Vehicle yaklaşımından farklı olarak araç fiziğinin Chaos Vehicle Movement Component üzerinden yönetilmesi

- Unreal'ın C++ ve Blueprint arasındaki çalışma modeli

## 5. Kapsam ve Önceliklendirme

48 saatlik süre nedeniyle önceliklendirme yapılmıştır.

Tamamlanan MVP

- Chaos Vehicle

- Keyboard vehicle controls

- Chase / cockpit camera

- Speed HUD

- 100 Hz binary UDP telemetry

- Python UDP receiver

- Sequence number ve packet loss detection

Tamamlanan Opsiyonel Özellik

- Hard braking event logging

- Collision event logging

Kapsam Dışında Bırakılanlar

- Virtual Sensor Simulation — Engel mesafesi ölçümü ve bu verinin telemetry paketine eklenmesi planlanmıştır ancak mevcut süre içerisinde çekirdek MVP ve Event Logging'in stabilitesini korumak önceliklendirilmiştir.

- External Hardware Input Abstraction — G29 veya harici UDP input entegrasyonu için genişletilebilir input mimarisi değerlendirilmiş ancak gerçek donanım entegrasyonu mevcut MVP kapsamına dahil edilmemiştir.

- Performance Analysis — Unreal Insights ile detaylı profiling çalışması gerçekleştirilmemiştir. Süre nedeniyle ayrı bir profiling raporu hazırlanması kapsam dışında bırakılmıştır.

Bu kapsam seçiminde amaç, daha fazla özellik eklemek yerine çalışan MVP'nin ve temel mimarinin güvenilirliğini sağlamaktır.

## 6. Zaman Çizelgesi

## 1. Gün – Cuma

- Unreal Engine 5.7 kurulumu ve proje oluşturma

- GitHub repository oluşturulması ve Git entegrasyonu

- Vehicle Template ve Chaos Vehicles mimarisinin incelenmesi

- Custom MiniVehiclePawn C++ sınıfının oluşturulması

- Enhanced Input sistemi

- Throttle, brake, steering ve handbrake

- Chase / cockpit camera geçişi

- Temel HUD ve hız göstergesi

## 2. Gün – Cumartesi

- Telemetry data modelinin oluşturulması

- Telemetry sampling

- Binary UDP publisher

- 100 Hz telemetry yayınlama

- Python UDP receiver

## 3. Gün – Pazar

- Packet sequence tracking ve packet loss detection

- Hard braking event logging

- Collision event logging

- Collision impact strength ve event aggregation

- Sistem testleri ve hata düzeltmeleri

- HUD kontrol bilgilerinin eklenmesi

- Git commit geçmişinin düzenlenmesi

- README hazırlanması

- Ekran kaydı / teknik demo videosu

- Final kontroller

## 7. 1 Haftalık Roadmap

- Virtual obstacle sensor ve telemetry entegrasyonu

- External hardware input abstraction

- Unreal Insights ile profiling ve performans analizi

- UDP communication abstraction ve daha kapsamlı packet validation

- Telemetry recording / replay sistemi

- Daha kapsamlı test ve edge-case handling

## 8. Öz Değerlendirme

### Gereksinim Bazlı Tamamlama

- Chaos Vehicle — Tamamlandı

- Keyboard controls — Tamamlandı

- UDP telemetry — Tamamlandı

- Python consumer — Tamamlandı

- Chase / Cockpit camera — Tamamlandı

- Speed HUD — Tamamlandı

- Event Logging — Tamamlandı

- Virtual Sensor — Kapsam dışında

- External Hardware Input — Kapsam dışında

- Performance Analysis — Kapsam dışında

### Teknik Öz Değerlendirme

- Çekirdek MVP gereksinimleri tamamlanmıştır.

- Öncelik, sınırlı süre içerisinde çalışan ve anlaşılır bir temel mimari oluşturmaya verilmiştir.

- Telemetry tarafında binary paket yapısı, 100 Hz yayın, sequence number ve packet loss detection uygulanmıştır.

- Event Logging opsiyonel kapsam içerisinde tamamlanmıştır.

- Virtual Sensor, external hardware input abstraction ve detaylı performance analysis süre ve önceliklendirme nedeniyle kapsam dışında bırakılmıştır.

- Kod tarafında sorumluluklar C++ component'leri üzerinden ayrıştırılmış, Blueprint daha çok Unreal'a özgü asset ve görsel yapılandırmalar için kullanılmıştır.

- Git geçmişi geliştirme adımlarını ayrı ve anlamlı commit'ler halinde yansıtmaktadır.

- Kalan geliştirmeler mevcut mimari üzerine eklenebilecek şekilde tasarlanmıştır.

## 9. AI / Kaynak Kullanımı

Geliştirme sürecinde teknik araştırma, Unreal Engine API kullanımı, hata ayıklama ve dokümantasyon konusunda ChatGPT yardımcı kaynak olarak kullanılmıştır.