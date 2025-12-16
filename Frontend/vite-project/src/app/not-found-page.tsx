import { useNavigate } from "react-router-dom";

import { Button } from "@/components/ui/button";
import { routingsMap } from "@/constants/routings";
import { cn } from "@/libs/utils/className";

export const NotFoundPage = () => {
    const navigate = useNavigate();
    const handleNavigateHome = () => {
        navigate(routingsMap.HOME.url);
    }

    return (
        <div className={cn("flex flex-col h-screen w-full items-center bg-slate-950 px-4 pt-24")}>
            <h1 className="text-2xl font-bold text-white mb-8">IoTシステム</h1>
            <h2 className="text-2xl font-bold text-white mb-8">404 Not Found</h2>
            <p className="text-2xl font-bold text-white mb-8">ページが見つかりませんでした。</p>
            <Button onClick={handleNavigateHome}>ホームに戻る</Button>
        </div>
    )
}
