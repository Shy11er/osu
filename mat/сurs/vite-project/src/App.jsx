import Papa from "papaparse";
import React, { useState } from "react";
import Plot from "react-plotly.js";

const App = () => {
    const [xValues, setXValues] = useState([]);
    const [yValues, setYValues] = useState([]);
    const [loaded, setLoaded] = useState(false);
    const [csvConvergence, setCsvConvergence] = useState([]);
    const [polynomial, setPolynomial] = useState("");
    const [interpolationResults, setInterpolationResults] = useState([]);

    // Статичные точки для каждой функции
    const sinX = [0, Math.PI / 6, Math.PI / 4, Math.PI / 2]; // x для синуса
    const sinY = [0, 0.5, 0.707, 1]; // y для синуса

    const cosX = [0, Math.PI / 6, Math.PI / 4, Math.PI / 2]; // x для косинуса
    const cosY = [1, 0.866, 0.707, 0]; // y для косинуса

    const expX = [0, 1, 2, 3]; // x для экспоненты
    const expY = [1, Math.exp(1), Math.exp(2), Math.exp(3)]; // y для экспоненты

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

                        // Вычисление таблицы сходимости для CSV файла
                        const convergenceTable = newtonInterpolation(
                            x,
                            y,
                            x[Math.floor(x.length / 2)] // Используем середину диапазона как x*
                        );
                        setCsvConvergence(convergenceTable);

                        // Вычисление конечного многочлена
                        const polynomialStr = constructPolynomial(x, y);
                        setPolynomial(polynomialStr);

                        // Вычисление значений функции в точках интерполяции
                        const interpolationPoints = [0.05, 0.07, 0.09, 0.15, 0.19];
                        const results = interpolationPoints.map((xi) => {
                            const table = newtonInterpolation(x, y, xi);
                            return { x: xi, y: table.at(-1).Pi };
                        });
                        setInterpolationResults(results);
                    } catch (error) {
                        alert("Ошибка при обработке файла: " + error.message);
                    }
                },
            });
        }
    };

    const newtonInterpolation = (x, y, xStar) => {
        const n = y.length;
        const coeffs = Array.from({ length: n }, () => Array(n).fill(0));
        const table = [];

        for (let i = 0; i < n; i++) {
            coeffs[i][0] = y[i];
        }

        for (let j = 1; j < n; j++) {
            for (let i = 0; i < n - j; i++) {
                coeffs[i][j] =
                    (coeffs[i + 1][j - 1] - coeffs[i][j - 1]) / (x[i + j] - x[i]);
            }
        }

        let result = coeffs[0][0];
        let product = 1;
        let previousResult = result;

        for (let i = 1; i < n; i++) {
            product *= xStar - x[i - 1];
            result += coeffs[0][i] * product;

            const convergence =
                i > 1 ? Math.abs(result - previousResult) : null;

            table.push({
                i: i + 1,
                Pi: result,
                deltaPi: convergence !== null ? convergence : "N/A",
            });

            previousResult = result;
        }

        return table;
    };

    const constructPolynomial = (x, y) => {
        const n = y.length;
        const coeffs = Array.from({ length: n }, () => Array(n).fill(0));

        for (let i = 0; i < n; i++) {
            coeffs[i][0] = y[i];
        }

        for (let j = 1; j < n; j++) {
            for (let i = 0; i < n - j; i++) {
                coeffs[i][j] =
                    (coeffs[i + 1][j - 1] - coeffs[i][j - 1]) / (x[i + j] - x[i]);
            }
        }

        let polynomial = `${coeffs[0][0]}`;

        for (let j = 1; j < n; j++) {
            let term = `${coeffs[0][j]}`;
            for (let k = 0; k < j; k++) {
                term += ` * (x - ${x[k]})`;
            }
            polynomial += ` + ${term}`;
        }

        return polynomial;
    };

    const renderConvergenceTable = (tableData) => (
        <table border="1" style={{ borderCollapse: "collapse", width: "100%" }}>
            <thead>
                <tr>
                    <th>Количество точек i</th>
                    <th>Pi(x*)</th>
                    <th>|Pi(x*) - Pi-1(x*)|</th>
                </tr>
            </thead>
            <tbody>
                {tableData.map((row, index) => (
                    <tr key={index}>
                        <td>{row.i}</td>
                        <td>{row.Pi.toFixed(10)}</td>
                        <td>
                            {typeof row.deltaPi === "number"
                                ? row.deltaPi.toFixed(10)
                                : "N/A"}
                        </td>
                    </tr>
                ))}
            </tbody>
        </table>
    );

    // Вычисление таблиц сходимости для каждой функции на точке 0.05
    const sinConvergenceAt005 = newtonInterpolation(sinX, sinY, 0.05);
    const cosConvergenceAt005 = newtonInterpolation(cosX, cosY, 0.05);
    const expConvergenceAt005 = newtonInterpolation(expX, expY, 0.05);

    return (
        <div style={{ padding: "20px" }}>
            <h1>Функции на разных графиках</h1>
            <input type="file" accept=".csv" onChange={handleFileUpload} />

            {/* График для синуса */}
            <h2>График синуса</h2>
            <Plot
                data={[
                    {
                        x: sinX,
                        y: sinY,
                        mode: "markers+lines",
                        type: "scatter",
                        name: "Синус",
                        marker: { color: "red", size: 10 },
                    },
                ]}
                layout={{
                    title: "Синус (y = sin(x))",
                    xaxis: { title: "X" },
                    yaxis: { title: "Y" },
                }}
            />
            <h3>Таблица сходимости для синуса</h3>
            {renderConvergenceTable(newtonInterpolation(sinX, sinY, Math.PI / 3))}

            <h3>Таблица сходимости для синуса (x* = 0.05)</h3>
            {renderConvergenceTable(sinConvergenceAt005)}

            {/* График для косинуса */}
            <h2>График косинуса</h2>
            <Plot
                data={[
                    {
                        x: cosX,
                        y: cosY,
                        mode: "markers+lines",
                        type: "scatter",
                        name: "Косинус",
                        marker: { color: "blue", size: 10 },
                    },
                ]}
                layout={{
                    title: "Косинус (y = cos(x))",
                    xaxis: { title: "X" },
                    yaxis: { title: "Y" },
                }}
            />
            <h3>Таблица сходимости для косинуса</h3>
            {renderConvergenceTable(newtonInterpolation(cosX, cosY, Math.PI / 3))}

            <h3>Таблица сходимости для косинуса (x* = 0.05)</h3>
            {renderConvergenceTable(cosConvergenceAt005)}

            {/* График для экспоненциальной функции */}
            <h2>График экспоненциальной функции</h2>
            <Plot
                data={[
                    {
                        x: expX,
                        y: expY,
                        mode: "markers+lines",
                        type: "scatter",
                        name: "Экспоненциальная",
                        marker: { color: "green", size: 10 },
                    },
                ]}
                layout={{
                    title: "Экспоненциальная функция (y = e^x)",
                    xaxis: { title: "X" },
                    yaxis: { title: "Y" },
                }}
            />
            <h3>Таблица сходимости для экспоненциальной функции</h3>
            {renderConvergenceTable(newtonInterpolation(expX, expY, 2))}

            <h3>Таблица сходимости для экспоненциальной функции (x* = 0.05)</h3>
            {renderConvergenceTable(expConvergenceAt005)}

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
                            {
                                x: [0.05, 0.07, 0.09, 0.15, 0.19],
                                y: interpolationResults.map((point) => point.y),
                                mode: "markers",
                                type: "scatter",
                                name: "Интерполяционные точки",
                                marker: { color: "orange", size: 8 },
                            },
                        ]}
                        layout={{
                            title: "Функция из CSV с точками интерполяции",
                            xaxis: { title: "X" },
                            yaxis: { title: "Y" },
                        }}
                    />
                    <h3>Результаты интерполяции для точек</h3>
                    <ul>
                        {interpolationResults.map((point, index) => (
                            <li key={index}>
                                y({point.x}) = {point.y.toFixed(10)}
                            </li>
                        ))}
                    </ul>

                    <h3>Таблица сходимости для функции из CSV</h3>
                    {renderConvergenceTable(csvConvergence)}

                    <h3>Конечный многочлен для функции из CSV</h3>
                    <p>{polynomial}</p>
                </>
            )}
        </div>
    );
};

export default App;
