import Papa from "papaparse";
import React, { useState } from "react";
import Plot from "react-plotly.js";

const App = () => {
    const [xValues, setXValues] = useState([]);
    const [yValues, setYValues] = useState([]);
    const [loaded, setLoaded] = useState(false);
    const [xStar, setXStar] = useState("");
    const [yStar, setYStar] = useState(null);
    const [convergenceTable, setConvergenceTable] = useState([]);

    // Функция для загрузки файла
    const handleFileUpload = (event) => {
        const file = event.target.files[0];
        if (file) {
            Papa.parse(file, {
                delimiter: ",",
                complete: (result) => {
                    try {
                        const data = result.data.map((row) =>
                            row.map((value) =>
                                parseFloat(
                                    value.toString().replace(",", ".").trim()
                                )
                            )
                        );

                        const x = data.map((row) => row[0]);
                        const y = data.map((row) => row[1]);

                        if (x.some(isNaN) || y.some(isNaN)) {
                            alert("Ошибка: Файл содержит некорректные данные!");
                            return;
                        }

                        setXValues(x);
                        setYValues(y);
                        setLoaded(true);
                        setYStar(null);
                        setConvergenceTable([]);
                    } catch (error) {
                        alert("Ошибка при обработке файла: " + error.message);
                    }
                },
            });
        }
    };

    // Функция интерполяции методом Ньютона
    const newtonInterpolation = (x, y, xStar) => {
        if (x.some(isNaN) || y.some(isNaN)) {
            throw new Error("Некорректные данные в массивах X или Y");
        }

        const n = y.length;
        const coeffs = Array.from({ length: n }, () => Array(n).fill(0));
        const table = [];

        for (let i = 0; i < n; i++) {
            coeffs[i][0] = y[i];
        }

        for (let j = 1; j < n; j++) {
            for (let i = 0; i < n - j; i++) {
                const denominator = x[i + j] - x[i];
                if (denominator === 0) {
                    throw new Error(
                        "Деление на ноль при вычислении коэффициентов"
                    );
                }
                coeffs[i][j] =
                    (coeffs[i + 1][j - 1] - coeffs[i][j - 1]) / denominator;
            }
        }

        let result = coeffs[0][0];
        let productTerm = 1;
        let previousResult = result;

        for (let i = 1; i < n; i++) {
            productTerm *= xStar - x[i - 1];
            result += coeffs[0][i] * productTerm;

            const rootsUsed = x.slice(0, i).join(", ");
            const convergence =
                i > 1 ? Math.abs(result - previousResult) : null;
            table.push({
                roots: rootsUsed,
                value: result,
                convergence,
            });

            previousResult = result;
        }

        return { result, table };
    };

    // Функция для вычисления результата
    const handleCalculate = () => {
        if (!loaded) {
            alert("Сначала загрузите CSV-файл!");
            return;
        }

        const xValue = parseFloat(xStar.replace(",", "."));
        if (isNaN(xValue)) {
            alert("Введите корректное значение для X*.");
            return;
        }

        try {
            const { result, table } = newtonInterpolation(
                xValues,
                yValues,
                xValue
            );
            setYStar(result);
            setConvergenceTable(table);
        } catch (error) {
            alert("Ошибка во время интерполяции: " + error.message);
        }
    };

    // Генерация точек для тестовых функций
    const testX = Array.from({ length: 100 }, (_, i) => i * 0.002 + 0.02);
    const testFunctions = {
        "Линейная (y = 2x + 0.01)": testX.map((x) => 2 * x + 0.01),
        "Квадратичная (y = x^2 + 0.01)": testX.map((x) => x ** 2 + 0.01),
        "Экспоненциальная (y = e^x - 1)": testX.map((x) => Math.exp(x) - 1),
    };

    return (
        <div style={{ padding: "20px" }}>
            <h1>Интерполяция методом Ньютона</h1>
            <input type="file" accept=".csv" onChange={handleFileUpload} />
            <div>
                <label>
                    Введите X* для интерполяции:
                    <input
                        type="text"
                        value={xStar}
                        onChange={(e) => setXStar(e.target.value)}
                    />
                </label>
                <button onClick={handleCalculate}>Вычислить</button>
            </div>
            {yStar !== null && (
                <p>
                    Интерполированное Y*: <strong>{yStar.toFixed(4)}</strong>
                </p>
            )}
            {loaded && (
                <Plot
                    data={[
                        {
                            x: xValues,
                            y: yValues,
                            mode: "markers+lines",
                            type: "scatter",
                            name: "Загруженные точки",
                            line: { color: "blue" },
                        },
                        yStar !== null && {
                            x: [parseFloat(xStar)],
                            y: [yStar],
                            mode: "markers",
                            type: "scatter",
                            name: `X* = ${xStar}, Y* = ${yStar.toFixed(4)}`,
                            marker: { color: "red", size: 10 },
                        },
                        ...Object.entries(testFunctions).map(([name, yValues]) => ({
                            x: testX,
                            y: yValues,
                            mode: "lines",
                            type: "scatter",
                            name,
                        })),
                    ].filter(Boolean)}
                    layout={{
                        title: "Интерполяция и тестовые функции",
                        xaxis: { title: "X" },
                        yaxis: { title: "Y" },
                    }}
                />
            )}
            {convergenceTable.length > 0 && (
                <div>
                    <h2>Таблица сходимости</h2>
                    <table border="1" style={{ borderCollapse: "collapse" }}>
                        <thead>
                            <tr>
                                <th>Использованные корни</th>
                                <th>Интерполированное значение</th>
                                <th>Сходимость</th>
                            </tr>
                        </thead>
                        <tbody>
                            {convergenceTable.map((row, index) => (
                                <tr key={index}>
                                    <td>{row.roots}</td>
                                    <td>{row.value.toFixed(10)}</td>
                                    <td>
                                        {row.convergence !== null
                                            ? row.convergence.toFixed(10)
                                            : "N/A"}
                                    </td>
                                </tr>
                            ))}
                        </tbody>
                    </table>
                </div>
            )}
        </div>
    );
};

export default App;
