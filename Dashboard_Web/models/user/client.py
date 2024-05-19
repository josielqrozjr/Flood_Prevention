from models import db
from models.db import datetime

class Client(db.Model):
    __tablename__ = 'client'

<<<<<<< Updated upstream
    id= db.Column('id', 
                  db.Integer, 
                  db.ForeignKey(Users.id), 
                  nullable=False, 
                  autoincrement=True, 
                  primary_key=True)
=======
    id = db.Column(db.Integer, db.ForeignKey('users.id'), primary_key=True)
>>>>>>> Stashed changes
    
    creation_date = db.Column(db.DateTime(), nullable = False, default=datetime.now())
    update_date = db.Column(db.DateTime(), nullable = False, default=datetime.now())
