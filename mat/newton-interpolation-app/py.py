import pandas as pd

# Создаем данные из изображения
data = {
    "x": [0.02, 0.04, 0.06, 0.08, 0.1, 0.12, 0.14, 0.16, 0.18, 0.2],
    "y": [0.0204, 0.0416, 0.063599, 0.086397, 0.109992, 0.134383, 0.159568, 0.185545, 0.212313, 0.239867]
}

# Создаем DataFrame
df = pd.DataFrame(data)

# Сохраняем в CSV файл
file_path = "./interpolation_points.csv"
df.to_csv(file_path, index=False, sep=";")
