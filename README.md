# 🎤 Mobile Mic Client

Mobile Mic Client, Flutter ile geliştirilmiş, mobil cihazı **Wi-Fi üzerinden canlı mikrofon** olarak kullanmayı sağlayan bir uygulamadır.  
Uygulama, sesi **Opus codec** ile sıkıştırarak düşük gecikme ve yüksek kalite ile bir sunucuya (`mic_server`) gönderir.

---

## 🚀 Features

- 📱 Flutter (Android / iOS)
- 🌐 Real-time audio streaming over Wi-Fi
- 🎧 Opus audio codec (low latency, high quality)
- 🎙️ Live microphone capture
- ⚡ Lightweight and efficient audio transmission
- 🔌 Socket-based communication (TCP / UDP)

---

## 🧩 Architecture

```text
[ Mobile Mic Client (Flutter) ]
            |
            |  Opus Encoded Audio (Wi-Fi)
            v
        [ mic_server ]
