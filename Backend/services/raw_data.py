from fastapi import FastAPI, Depends, HTTPException, APIRouter
import uvicorn
from datetime import datetime
import ulid


from schemas.raw_data import RawDataResponse, PostRawDataRequest

from models.raw_data import RawData
from settings import SessionLocal

from sqlalchemy.orm import Session

from algorithm.check_outeliers import check_outliers

prefix = "/raw-data"
router = APIRouter()

def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()

@router.get("", response_model=list[RawDataResponse])
async def get_raw_data(db: Session = Depends(get_db)):
    try:
        return db.query(RawData).all()
    except Exception as e:
        raise HTTPException(status_code=400, detail=str(e))
        
@router.post("")
async def append_raw_data(request: PostRawDataRequest, db: Session = Depends(get_db)):
    try:
        date_time = request.date_time
        illuminance = request.illuminance
        is_people_detected = request.is_people_detected
        is_outlier : bool = check_outliers(date_time, illuminance, is_people_detected)
        new_data = RawData(id=ulid.ulid(), mode=request.mode, date_time=date_time, illuminance=illuminance, is_people_detected=is_people_detected, is_outlier=is_outlier)
        db.add(new_data)
        db.commit()
        db.refresh(new_data)
        return
    except Exception as e:
        db.rollback()
        print(f"Error in append_raw_data: {e}")
        raise HTTPException(status_code=400, detail=str(e))

@router.delete("")
async def delete_raw_data(id: str, db: Session = Depends(get_db)):
    try:
        db.query(RawData).filter(RawData.id == id).delete()
        db.commit()
    except Exception as e:
        db.rollback()
        raise HTTPException(status_code=400, detail=str(e))


