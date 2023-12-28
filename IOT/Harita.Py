import sys
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QImage, QPixmap
from PyQt5.QtWidgets import QApplication, QMainWindow, QLabel, QPushButton,QWidget
from PyQt5.QtGui import QFont
import io
import folium # pip install folium
from PyQt5.QtWidgets import QApplication, QWidget, QHBoxLayout, QVBoxLayout
from PyQt5.QtWebEngineWidgets import QWebEngineView,QWebEngineSettings # pip install PyQtWebEngine
import numpy as np
from VeriOku import df
from geopy.geocoders import Nominatim
from datetime import datetime

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("NBR Map in Turkey")
    
        layout = QVBoxLayout()
        self.setLayout(layout)
        
        self.setGeometry(0,0, 1920,1500)
        self.setFixedSize(1920,1500)
        coordinate = (40.75,30.33)
        m = folium.Map(
           location=coordinate,
           zoom_start=10,  # Zoom seviyesini değiştirebilirsiniz
           tiles='OpenStreetMap'
        )

        coordinates = list(zip(df['Latitude'], df['Longitude']))
        for coord in coordinates:
         Latitude, Longitude = coord   # Koordinatları ayırın
         marker=folium.Marker(location=[Latitude,Longitude], popup='Deprem Noktası', icon=folium.Icon(icon='circle', color='red')).add_to(m)
         anlik_tarih = datetime.now()
         yil = str(anlik_tarih.year)
         ay = anlik_tarih.month
         gun = anlik_tarih.day
         # Bilgileri ekrana yazdır
         tarih = f"{gun}/{ay}/{yil}"
         geolocator = Nominatim(user_agent="ornek_reverse_geocode")
         lokasyon = geolocator.reverse((Latitude, Longitude), language='tr')
         buyukluk = df.loc[df['Latitude'] == Latitude, 'sallanti_buyuklugu'].values[0]
         if lokasyon:
                adres = lokasyon.address
                popup_content = f"""
                <b>Deprem Detayları</b><br>
                <i>Adres:</i> {adres}</b><br>
                <i>Buyukluk:</i>{buyukluk}</b><br>
                <i>Tarih:</i>{tarih}<br>
                """
                marker = folium.Marker(location=[Latitude,Longitude], popup=folium.Popup(popup_content, max_width=300), icon=folium.Icon(icon='circle', color='red')).add_to(m)
        # save map data to data object
        data = io.BytesIO()
        m.save(data, close_file=False)

        # Etiketler oluştur
        self.label = QWebEngineView(self)
        self.label.setHtml(data.getvalue().decode())
        self.label.setGeometry(10, 2, 1920,1500)
        self.label.setStyleSheet("\n"
"background-color: rgb(150,150,150);\n"
"border-radius:20px;\n"
"")
        

if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())


    
     
    
