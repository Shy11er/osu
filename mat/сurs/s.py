import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from tkinter import Tk, Label, Button, Entry, filedialog, Toplevel, Text, Scrollbar, RIGHT, Y, BOTH, font


class NewtonInterpolationApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Интерполяция методом Ньютона")

        self.x_values = None
        self.y_values = None
        self.loaded = False

        # GUI Elements
        self.load_button = Button(root, text="Загрузить CSV", command=self.load_csv)
        self.load_button.pack()

        self.label = Label(root, text="Введите X* для интерполяции:")
        self.label.pack()

        self.entry = Entry(root)
        self.entry.pack()

        self.calc_button = Button(root, text="Вычислить", command=self.calculate)
        self.calc_button.pack()

        self.result_label = Label(root, text="")
        self.result_label.pack()

        # Plot
        self.fig, self.ax = plt.subplots()
        self.ax.set_title("Интерполяция методом Ньютона")
        self.ax.set_xlabel("X")
        self.ax.set_ylabel("Y")
        self.ax.grid(True, linestyle='--', alpha=0.7)

    def load_csv(self):
        filepath = filedialog.askopenfilename(filetypes=[("CSV files", "*.csv")])
        if filepath:
            try:
                # Load the CSV with a semicolon delimiter
                data = pd.read_csv(filepath, delimiter=";", header=None)
                self.x_values = data.iloc[:, 0].values
                self.y_values = data.iloc[:, 1].values
                self.loaded = True

                # Plot the loaded points
                self.ax.clear()
                self.ax.scatter(self.x_values, self.y_values, color='blue', label='Точки из файла')
                self.ax.legend()
                self.ax.set_title("Интерполяция методом Ньютона")
                self.ax.set_xlabel("X")
                self.ax.set_ylabel("Y")
                self.ax.grid(True, linestyle='--', alpha=0.7)
                plt.draw()

                self.result_label.config(text="Данные успешно загружены!")
            except Exception as e:
                self.result_label.config(text=f"Ошибка загрузки файла: {e}")

    def calculate(self):
        if not self.loaded:
            self.result_label.config(text="Сначала загрузите файл с данными!")
            return

        try:
            x_star = float(self.entry.get())
            y_star, convergence_table = self.newton_interpolation(self.x_values, self.y_values, x_star)

            # Update plot with the new point
            self.ax.scatter([x_star], [y_star], color='red', label=f'X* = {x_star}, Y* = {y_star:.4f}')
            self.ax.legend()
            plt.draw()

            self.result_label.config(text=f"Y* = {y_star:.4f}")

            # Show convergence table
            self.show_convergence_table(convergence_table)
        except ValueError:
            self.result_label.config(text="Введите корректное числовое значение X*!")

    def newton_interpolation(self, x, y, x_star):
        n = len(y)
        coeffs = np.zeros((n, n))
        coeffs[:, 0] = y

        convergence_table = []

        for j in range(1, n):
            for i in range(n - j):
                coeffs[i, j] = (coeffs[i + 1, j - 1] - coeffs[i, j - 1]) / (x[i + j] - x[i])

        result = coeffs[0, 0]
        product_term = 1.0
        previous_result = result

        for i in range(1, n):
            product_term *= (x_star - x[i - 1])
            result += coeffs[0, i] * product_term

            # Record convergence data
            roots_used = ', '.join(map(str, x[:i]))
            convergence = abs(result - previous_result) if i > 1 else None
            convergence_table.append((i + 1, roots_used, result, convergence))

            previous_result = result

        return result, convergence_table

    def show_convergence_table(self, convergence_table):
        table_window = Toplevel(self.root)
        table_window.title("Таблица сходимости")

        custom_font = font.Font(family="Helvetica", size=12)

        text_area = Text(table_window, wrap='none')
        scrollbar_y = Scrollbar(table_window, orient='vertical', command=text_area.yview)
        text_area.configure(yscrollcommand=scrollbar_y.set)

        # Add headers
        text_area.insert('end', f"{'Количество корней':<20}||{'Корни':<75}||{'Значение':<20}||{'Сходимость':<20}\n")
        text_area.insert('end', "=" * 140 + "\n")

        # Add table rows
        for row in convergence_table:
            formatted_convergence = f"{row[3]:.10f}" if row[3] is not None else "N/A"
            text_area.insert('end', f"{row[0]:<20}||{row[1]:<75}||{row[2]:<20.10f}||{formatted_convergence:<20}\n")

        text_area.pack(side='left', fill=BOTH, expand=True)
        scrollbar_y.pack(side=RIGHT, fill=Y)


if __name__ == "__main__":
    root = Tk()
    app = NewtonInterpolationApp(root)
    plt.ion()
    plt.show()
    root.mainloop()
