import Papa from "papaparse";
import React, { useState } from "react";
import Plot from "react-plotly.js";

const App = () => {
    const [xValues, setXValues] = useState([]);
    const [yValues, setYValues] = useState([]);
    const [loaded, setLoaded] = useState(false);
    const [yStars, setYStars] = useState([]);
    const [convergenceTables, setConvergenceTables] = useState([]);

    // Задаем несколько значений для X*
    const xStars = [0.05, 0.07, 0.09, 0.15, 0.19];

    // Функция для загрузки файла
    const handleFileUpload = (event) => {
        const file = event.target.files[0];
        if (file) {
            Papa.parse(file, {
                delimiter: ",", // используем запятую как разделитель
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
                        setYStars([]);
                        setConvergenceTables([]);
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

    // Функция для вычисления результатов для нескольких значений X*
    const handleCalculate = () => {
        if (!loaded) {
            alert("Сначала загрузите CSV-файл!");
            return;
        }

        const yResults = [];
        const tables = [];

        try {
            // Вычисляем для каждого значения X* и генерируем таблицу сходимости
            xStars.forEach((xStar) => {
                const { result, table } = newtonInterpolation(
                    xValues,
                    yValues,
                    xStar
                );
                yResults.push({ xStar, yStar: result });
                tables.push(table);
            });

            setYStars(yResults);
            setConvergenceTables(tables);
        } catch (error) {
            alert("Ошибка во время интерполяции: " + error.message);
        }
    };

    // Генерация точек для тестовых функций с более широким диапазоном
    const testX = Array.from({ length: 100 }, (_, i) => i * 0.02); // Более широкий диапазон для синуса и косинуса

    // Генерация тестовых функций
    const testFunctions = {
        "Синусная (y = sin(x))": testX.map((x) => Math.sin(x)),
        "Косинусная (y = cos(x))": testX.map((x) => Math.cos(x)),
        "Экспоненциальная (y = e^x)": testX.map((x) => Math.exp(x)),
    };

    return (
        <div style={{ padding: "20px" }}>
            <h1>Функции на разных графиках</h1>
            <input type="file" accept=".csv" onChange={handleFileUpload} />
            <button onClick={handleCalculate}>Вычислить</button>

            {/* График для косинуса */}
            <h2>График косинуса</h2>
            <Plot
                data={[
                    {
                        x: testX,
                        y: testFunctions["Косинусная (y = cos(x))"],
                        mode: "lines",
                        type: "scatter",
                        name: "Косинус",
                    },
                ]}
                layout={{
                    title: "Косинус (y = cos(x))",
                    xaxis: { title: "X" },
                    yaxis: { title: "Y" },
                }}
            />
            {convergenceTables.length > 0 && (
                <div>
                    <h3>Таблица сходимости для косинуса</h3>
                    <table
                        border="1"
                        style={{ borderCollapse: "collapse", width: "100%" }}
                    >
                        <thead>
                            <tr>
                                <th>Использованные корни</th>
                                <th>Интерполированное значение</th>
                                <th>Сходимость</th>
                            </tr>
                        </thead>
                        <tbody>
                            {convergenceTables[0].map((row, rowIndex) => (
                                <tr key={rowIndex}>
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

            {/* График для синуса */}
            <h2>График синуса</h2>
            <Plot
                data={[
                    {
                        x: testX,
                        y: testFunctions["Синусная (y = sin(x))"],
                        mode: "lines",
                        type: "scatter",
                        name: "Синус",
                    },
                ]}
                layout={{
                    title: "Синус (y = sin(x))",
                    xaxis: { title: "X" },
                    yaxis: { title: "Y" },
                }}
            />
            {convergenceTables.length > 1 && (
                <div>
                    <h3>Таблица сходимости для синуса</h3>
                    <table
                        border="1"
                        style={{ borderCollapse: "collapse", width: "100%" }}
                    >
                        <thead>
                            <tr>
                                <th>Использованные корни</th>
                                <th>Интерполированное значение</th>
                                <th>Сходимость</th>
                            </tr>
                        </thead>
                        <tbody>
                            {convergenceTables[1].map((row, rowIndex) => (
                                <tr key={rowIndex}>
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

            {/* График для экспоненциальной функции */}
            <h2>График экспоненциальной функции</h2>
            <Plot
                data={[
                    {
                        x: testX,
                        y: testFunctions["Экспоненциальная (y = e^x)"],
                        mode: "lines",
                        type: "scatter",
                        name: "Экспоненциальная",
                    },
                ]}
                layout={{
                    title: "Экспоненциальная функция (y = e^x)",
                    xaxis: { title: "X" },
                    yaxis: { title: "Y" },
                }}
            />
            {convergenceTables.length > 2 && (
                <div>
                    <h3>Таблица сходимости для экспоненциальной функции</h3>
                    <table
                        border="1"
                        style={{ borderCollapse: "collapse", width: "100%" }}
                    >
                        <thead>
                            <tr>
                                <th>Использованные корни</th>
                                <th>Интерполированное значение</th>
                                <th>Сходимость</th>
                            </tr>
                        </thead>
                        <tbody>
                            {convergenceTables[2].map((row, rowIndex) => (
                                <tr key={rowIndex}>
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

            {/* График для функции из CSV */}
            {loaded && (
                <>
                    <h2>График из CSV файла</h2>
                    <Plot
                        data={[
                            {
                                x: xValues,
                                y: yValues,
                                mode: "markers+lines",
                                type: "scatter",
                                name: "Функция из CSV",
                                line: { color: "blue" },
                            },
                        ]}
                        layout={{
                            title: "Функция из CSV",
                            xaxis: { title: "X" },
                            yaxis: { title: "Y" },
                        }}
                    />
                    {convergenceTables.length > 3 && (
                        <div>
                            <h3>Таблица сходимости для CSV функции</h3>
                            <table
                                border="1"
                                style={{ borderCollapse: "collapse", width: "100%" }}
                            >
                                <thead>
                                    <tr>
                                        <th>Использованные корни</th>
                                        <th>Интерполированное значение</th>
                                        <th>Сходимость</th>
                                    </tr>
                                </thead>
                                <tbody>
                                    {convergenceTables[3].map((row, rowIndex) => (
                                        <tr key={rowIndex}>
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
                </>
            )}
        </div>
    );
};

export default App;
