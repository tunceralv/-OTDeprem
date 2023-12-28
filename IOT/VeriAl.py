import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sb
import folium
import firebase_admin
from firebase_admin import credentials, db

class VeriAl:
    def __init__(self):
        # Firebase proje anahtarınızı içeren JSON dosyasını belirtin
        cred = credentials.Certificate("serviceAccountKey.json")

        # Firebase uygulamasını başlatın
        firebase_admin.initialize_app(cred, {'databaseURL': 'https://proje1-eb887-default-rtdb.europe-west1.firebasedatabase.app/'})
        
    def get_data(self):
        # Veritabanından belirli bir yol (path) üzerindeki veriyi çekin
        data_path = 'Earthquake'
        ref = db.reference(data_path)
        data = ref.get()

        if data is not None:
            # Veri varsa, işlemlerinizi gerçekleştirin
            if data:
                file_path = 'veriler.csv'
                
                # Veriyi bir DataFrame'e çevirin
                df = pd.DataFrame(data).transpose()

                # CSV dosyasını oluşturun veya var olan dosyayı açın
                try:
                    existing_df = pd.read_csv(file_path)
                except FileNotFoundError:
                    existing_df = pd.DataFrame(columns=df.columns)  # CSV dosyasının başlıkları

                # Yeni veriyi DataFrame'e ekleyin
                updated_df = existing_df.append(df)

                # DataFrame'i CSV dosyasına yazın
                updated_df.to_csv(file_path, index=False)
                
                print("Veriler başarıyla CSV dosyasına eklendi.")
            else:
                print("Veri bulunamadı.")

if __name__ == '__main__':
    veri_al = VeriAl()
    veri_al.get_data() 
    