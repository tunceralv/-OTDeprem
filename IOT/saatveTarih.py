from datetime import datetime

# Şu anki tarih ve saat bilgisini al

anlik_tarih = datetime.now()
yil = str(anlik_tarih.year)
ay = anlik_tarih.month
gun = anlik_tarih.day

# Bilgileri ekrana yazdır
print(f"{gun}/{ay}/{yil}")

