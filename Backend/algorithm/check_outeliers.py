from settings import SessionLocal
from models.raw_data import RawData
from datetime import datetime
import pandas as pd
from sklearn.ensemble import IsolationForest
from fastapi import HTTPException

def check_outliers(date_time: str, illuminance: int, is_people_detected: bool) -> bool:
    """
    Isolation Forestを使用して照度、時間、人感検知データから外れ値を検出し、
    最新のデータが外れ値かどうかを判定する関数
    """
    db = SessionLocal()
    try:
        data_query = db.query(RawData).all()
        if len(data_query) < 2:
            return False
            
        data_list = []
        for d in data_query:
            dt = d.date_time if isinstance(d.date_time, datetime) else datetime.fromisoformat(str(d.date_time))
            data_list.append({
                "illuminance": d.illuminance,
                "hour": dt.hour,
                "is_people_detected_int": 1 if d.is_people_detected else 0
            })
        
        df = pd.DataFrame(data_list)
        X = df[['illuminance', 'hour', 'is_people_detected_int']].values

        clf = IsolationForest(contamination=0.05, random_state=42)
        clf.fit(X)

        # 今回の入力データを数値化
        in_dt = date_time if isinstance(date_time, datetime) else datetime.fromisoformat(str(date_time))
        in_hour = in_dt.hour
        in_people = 1 if is_people_detected else 0
        
        is_outliers = clf.predict([[illuminance, in_hour, in_people]])
        return bool(is_outliers[0] == -1)
    except Exception as e:
        print(f"Error in ML processing: {e}")
        raise HTTPException(status_code=500, detail=f"Machine Learning Error: {str(e)}")
    finally:
        db.close()
