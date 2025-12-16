import "@/index.css";
import React from "react";
import { createRoot } from "react-dom/client";
import { BrowserRouter, Route, Routes } from "react-router-dom";

import { HomePage, NotFoundPage } from "./index";

import { routingsMap } from "@/constants/routings";

createRoot(document.getElementById("root")!).render(
    <React.StrictMode>
        <BrowserRouter>
            <Routes>
                <Route path={routingsMap.HOME.url} element={<HomePage />} />
                <Route path="*" element={<NotFoundPage />} />
            </Routes>
        </BrowserRouter>
    </React.StrictMode>
);
