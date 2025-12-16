import { useEffect, useState } from "react";
import { CartesianGrid, Line, LineChart, ResponsiveContainer, Tooltip, XAxis, YAxis, Legend, Brush } from "recharts";

type RawData = {
    id: string;
    mode: number;
    date_time: string;
    illuminance: number;
    is_people_detected: boolean;
};

export const RawDataGraph = () => {
    const [data, setData] = useState<RawData[]>([]);

    const fetchData = async () => {
        try {
            const response = await fetch("http://localhost:8000/raw-data");
            if (!response.ok) {
                console.error("Failed to fetch data");
                return;
            }
            const json: RawData[] = await response.json();
            // Sort by date just in case, though usually DB returns in order or insertion order
            const sorted = json.sort((a, b) => new Date(a.date_time).getTime() - new Date(b.date_time).getTime());
            setData(sorted);
        } catch (error) {
            console.error("Error fetching data:", error);
        }
    };

    useEffect(() => {
        fetchData();
        const interval = setInterval(fetchData, 10000); // Poll every 10 seconds
        return () => clearInterval(interval);
    }, []);

    // Transform data for chart: convert boolean to number for display
    const chartData = data.map(item => ({
        ...item,
        // Convert ISO string to readable time
        time: new Date(item.date_time).toLocaleTimeString(),
        people_detected_val: item.is_people_detected ? 1 : 0
    }));

    const startIndex = Math.max(0, data.length - 180);

    return (
        <div className="w-full h-[400px] bg-white/5 rounded-xl p-4">
            <ResponsiveContainer width="100%" height="100%">
                <LineChart data={chartData}>
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
                        contentStyle={{ backgroundColor: '#333', border: 'none', color: '#fff' }}
                    />
                    <Legend />
                    <Line
                        yAxisId="left"
                        type="monotone"
                        dataKey="illuminance"
                        stroke="#8884d8"
                        name="照度"
                        dot={false}
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
                        />
                    )}
                </LineChart>
            </ResponsiveContainer>
        </div>
    );
};