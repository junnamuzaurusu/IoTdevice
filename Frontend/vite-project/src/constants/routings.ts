export type Routing = "HOME" | "NOT_FOUND"

export interface Route {
    url: string;
    title: string;
    parent: Routing | null;
}

export const routingsMap: Record<Routing, Route> = {
    "HOME": {
        "url": "/",
        "title": "Home",
        "parent": null
    },
    "NOT_FOUND": {
        "url": "*",
        "title": "Not Found",
        "parent": null
    }
}

export const getParentURL: (routing: Routing) => string = (routing) => {
    return routingsMap[routing].parent ? routingsMap[routingsMap[routing].parent].url : "/";
}
