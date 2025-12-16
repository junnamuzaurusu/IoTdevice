import { cn } from "@/libs/utils/className";
import { Tab, TabGroup, TabList, TabPanel, TabPanels } from "@headlessui/react"
import { RawDataGraph } from "./_components/table/raw-data-graph";
import { MachineLearningGraph } from "./_components/table/machine-learning-graph";

const categories = [
    {
        title: "センサーから取得されたデータ",
        content: <RawDataGraph />
    },
    {
        title: "機械学習の結果",
        content: <MachineLearningGraph />
    }
]


export const HomePage = () => {
    return (
        <div className={cn("flex flex-col h-screen w-full items-center bg-slate-950 px-4 pt-24")}>
            <h1 className="text-2xl font-bold text-white mb-8">IoTシステム</h1>
            <div className="w-full max-w-5xl">
                <TabGroup>
                    <TabList className="flex gap-4 border-b border-white/10 pb-2">
                        {categories.map(({ title }) => (
                            <Tab
                                key={title}
                                className="rounded-md px-3 py-2 text-sm font-medium text-white/70 hover:text-white focus:outline-none data-[selected]:bg-white/10 data-[selected]:text-white transition-colors"
                            >
                                {title}
                            </Tab>
                        ))}
                    </TabList>
                    <TabPanels className="mt-6">
                        {categories.map(({ title, content }) => (
                            <TabPanel key={title} className="rounded-xl w-full bg-white/5 p-6 shadow-inner ring-1 ring-white/10">
                                {content}
                            </TabPanel>
                        ))}
                    </TabPanels>
                </TabGroup>
            </div>
        </div>
    )
}