import React, { useState } from "react";
import { Line } from "react-chartjs-2";
import * as d3 from "d3";
import {
    Chart as ChartJS,
    CategoryScale,
    LinearScale,
    PointElement,
    LineElement,
    Tooltip,
    Legend,
} from "chart.js";

ChartJS.register(
    CategoryScale,
    LinearScale,
    PointElement,
    LineElement,
    Tooltip,
    Legend
);

const App = () => {
    const [datasets, setDatasets] = useState([]);
    const [result, setResult] = useState(null);
    const [xStar, setXStar] = useState(0);

    const handleFileChange = async (e) => {
        const file = e.target.files[0];
        if (!file) return;

        const text = await file.text();
        const rows = d3.csvParseRows(text, (row) => row.map(Number));
        const xData = rows.map((row) => row[0]);
        const yData = rows.map((row) => row[1]);

        addDataset("Загруженные данные", xData, yData, "blue");
    };

    const calculateNewtonCoefficients = (x, y) => {
        const n = y.length;
        const coeffs = [...y];

        for (let j = 1; j < n; j++) {
            for (let i = n - 1; i >= j; i--) {
                coeffs[i] = (coeffs[i] - coeffs[i - 1]) / (x[i] - x[i - j]);
            }
        }

        return coeffs;
    };

    const interpolate = (x, coeffs, xStar) => {
        let result = coeffs[0];
        let product = 1;

        for (let i = 1; i < coeffs.length; i++) {
            product *= xStar - x[i - 1];
            result += coeffs[i] * product;
        }

        return result;
    };

    const handleInterpolation = () => {
        if (datasets.length === 0 || xStar === null) {
            alert(
                "Пожалуйста, загрузите данные или выберите тестовые данные и введите X*!"
            );
            return;
        }

        const latestDataset = datasets[datasets.length - 1];
        const coefficients = calculateNewtonCoefficients(
            latestDataset.x,
            latestDataset.y
        );
        const yStar = interpolate(latestDataset.x, coefficients, xStar);

        setResult(yStar);
        addDataset(
            `Интерполированная точка (X*: ${xStar}, Y*: ${yStar.toFixed(4)})`,
            [xStar],
            [yStar],
            "red",
            true
        );
    };

    const loadTest1 = () => {
        const testX = [-1, -0.5, 0, 0.5, 1];
        const testY = [1, 0.25, 0, 0.25, 1]; // Парабола
        addDataset("Тест 1 (Парабола)", testX, testY, "green");
    };

    const loadTest2 = () => {
        const testX = [1, 2, 3, 4, 5];
        const testY = [1, 0.5, 0.3333, 0.25, 0.2]; // Гипербола
        addDataset("Тест 2 (Гипербола)", testX, testY, "purple");
    };

    const loadTest3 = () => {
        const testX = [0, 1, 2, 3, 4];
        const testY = [0, 1, 8, 27, 64]; // Кубическая функция
        addDataset("Тест 3 (Кубическая функция)", testX, testY, "orange");
    };

    const loadTest4 = () => {
        const testX = [0, Math.PI / 2, Math.PI, (3 * Math.PI) / 2, 2 * Math.PI];
        const testY = [0, 1, 0, -1, 0]; // Синус
        addDataset("Тест 4 (Синус)", testX, testY, "cyan");
    };

    const options = {
        scales: {
            x: {
                min: -1,
                max: 6,
            },
            y: {
                min: -1.5,
                max: 70,
            },
        },
    };

    const addDataset = (label, x, y, color, isPoint = false) => {
        const newDataset = {
            label,
            x,
            y,
            borderColor: color,
            backgroundColor: color,
            pointStyle: isPoint ? "cross" : "circle",
            pointRadius: isPoint ? 10 : 5,
            fill: false,
        };
        setDatasets((prev) => [...prev, newDataset]);
    };

    const data = {
        labels: datasets.reduce((allLabels, dataset) => {
            return Array.from(new Set([...allLabels, ...dataset.x]));
        }, []),
        datasets: datasets.map((dataset) => ({
            label: dataset.label,
            data: dataset.x.map((x, i) => ({ x, y: dataset.y[i] })),
            borderColor: dataset.borderColor,
            backgroundColor: dataset.backgroundColor,
            pointStyle: dataset.pointStyle,
            pointRadius: dataset.pointRadius,
        })),
    };

    return (
        <div style={{ padding: "20px", fontFamily: "Arial, sans-serif" }}>
            <h1>Интерполяция методом Ньютона</h1>

            <div style={{ marginBottom: "20px" }}>
                <input type="file" accept=".csv" onChange={handleFileChange} />
                <button onClick={loadTest1} style={{ marginLeft: "10px" }}>
                    Тест 1 (Парабола)
                </button>
                <button onClick={loadTest2} style={{ marginLeft: "10px" }}>
                    Тест 2 (Гипербола)
                </button>
                <button onClick={loadTest3} style={{ marginLeft: "10px" }}>
                    Тест 3 (Кубическая функция)
                </button>
                <button onClick={loadTest4} style={{ marginLeft: "10px" }}>
                    Тест 4 (Синус)
                </button>
            </div>

            <div style={{ margin: "20px 0" }}>
                <label>
                    Введите X*:
                    <input
                        type="number"
                        value={xStar}
                        onChange={(e) =>
                            setXStar(parseFloat(e.target.value) || 0)
                        }
                        style={{
                            marginLeft: "10px",
                            padding: "5px",
                            fontSize: "1rem",
                        }}
                    />
                </label>
                <button
                    onClick={handleInterpolation}
                    style={{ marginLeft: "10px" }}
                >
                    Рассчитать
                </button>
            </div>

            {result !== null && <p>Результат: Y* = {result.toFixed(4)}</p>}

            <div style={{ width: "800px", height: "400px", marginTop: "20px" }}>
                <Line options={options} data={data} />
            </div>
        </div>
    );
};

export default App;
