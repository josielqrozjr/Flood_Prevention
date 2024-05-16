from models import db
from models.user.users import Users
from models.db import datetime

class Admin(db.Model):
    __tablename__ = 'admin'

    id= db.Column('id', 
                  db.Integer, 
                  db.ForeignKey(Users.id), 
                  nullable=False, 
                  autoincrement=True, 
                  primary_key=True)
    
    role= db.Column(db.String(100), nullable= False)
    creation_date = db.Column(db.DateTime(), nullable = False, default=datetime.now())
    update_date = db.Column(db.DateTime(), nullable = False, default=datetime.now())
