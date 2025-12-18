import { useEffect, useState } from "react";
import { CartesianGrid, Line, ComposedChart, Scatter, ResponsiveContainer, Tooltip, XAxis, YAxis, Legend, Brush } from "recharts";

type MachineLearningData = {
    id: string;
    mode: number;
    date_time: string;
    illuminance: number;
    is_people_detected: boolean;
    is_outlier: boolean;
};

export const MachineLearningGraph = () => {
    const [data, setData] = useState<MachineLearningData[]>([]);

    const fetchData = async () => {
        try {
            const response = await fetch("http://localhost:8000/raw-data");
            if (!response.ok) {
                console.error("Failed to fetch data");
                return;
            }
            const json: MachineLearningData[] = await response.json();
            const sorted = json.sort((a, b) => new Date(a.date_time).getTime() - new Date(b.date_time).getTime());
            setData(sorted);
        } catch (error) {
            console.error("Error fetching data:", error);
        }
    };

    useEffect(() => {
        fetchData();
        const interval = setInterval(fetchData, 10000);
        return () => clearInterval(interval);
    }, []);

    const chartData = data.map(item => ({
        ...item,
        time: new Date(item.date_time).toLocaleTimeString(),
        people_detected_val: item.is_people_detected ? 1 : 0,
        outlier_val: item.is_outlier ? item.illuminance : null
    }));

    const startIndex = Math.max(0, data.length - 180);

    return (
        <div className="w-full h-[400px] bg-white/5 rounded-xl p-4">
            <ResponsiveContainer width="100%" height="100%">
                <ComposedChart data={chartData}>
                    <CartesianGrid strokeDasharray="3 3" stroke="#444" />
                    <XAxis
                        dataKey="time"
                        stroke="#888"
                        tick={{ fill: '#888' }}
                        interval="preserveStartEnd"
                        minTickGap={30}
                    />
                    <YAxis
                        yAxisId="left"
                        stroke="#8884d8"
                        tick={{ fill: '#8884d8' }}
                        label={{ value: '照度 (lux)', angle: -90, position: 'insideLeft', fill: '#8884d8' }}
                    />
                    <YAxis
                        yAxisId="right"
                        orientation="right"
                        stroke="#82ca9d"
                        tick={{ fill: '#82ca9d' }}
                        domain={[0, 1.2]}
                        ticks={[0, 1]}
                        label={{ value: '人感 (0/1)', angle: 90, position: 'insideRight', fill: '#82ca9d' }}
                    />
                    <Tooltip
                        contentStyle={{ backgroundColor: '#1f2937', border: 'none', borderRadius: '8px', color: '#fff' }}
                        itemStyle={{ color: '#fff' }}
                    />
                    <Legend />
                    <Line
                        yAxisId="left"
                        type="monotone"
                        dataKey="illuminance"
                        stroke="#8884d8"
                        strokeWidth={2}
                        name="照度"
                        dot={false}
                        activeDot={{ r: 6 }}
                    />
                    <Scatter
                        yAxisId="left"
                        name="外れ値"
                        dataKey="outlier_val"
                        fill="#ef4444"
                    />
                    <Line
                        yAxisId="right"
                        type="step"
                        dataKey="people_detected_val"
                        stroke="#82ca9d"
                        name="人感センサー"
                        dot={false}
                    />
                    {data.length > 0 && (
                        <Brush
                            dataKey="time"
                            height={30}
                            stroke="#8884d8"
                            startIndex={startIndex}
                            fill="#1f2937"
                        />
                    )}
                </ComposedChart>
            </ResponsiveContainer>
        </div>
    );
};