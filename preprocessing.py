#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Jun  9 15:15:23 2020

@author: raissapimentel
"""

import pandas as pd
import numpy as np
import random
import os

data = pd.read_csv('original_data/intraday_15min.csv')

data['date'] = data['timestamp'].apply(lambda x: x.split(' ')[0])
data['hour'] = data['timestamp'].apply(lambda x: x.split(' ')[1])

data['month'] = data['date'].apply(lambda x: x.split('-')[1])

data = data[data['month'] == '05']

data.drop(columns=['timestamp','month','volume','open'],inplace=True)

data['hour'] = data['hour'].apply(lambda x: str(int(x.split(':')[0]) + 1) + ':' + x.split(':')[1])

data['price'] = data['close']

data_mod = data.copy()

for index in data.index:
    hour = data.loc[index,'hour']
    high = data.loc[index,'high']
    low = data.loc[index,'low']
    date = data.loc[index,'date']
    if hour != '10:00':
        for i in range(1,15):
            if hour.split(':')[1] != '00':
                new_minute = int(hour.split(':')[1]) - i
                new_minute = '0' + str(new_minute) if (new_minute < 10) else str(new_minute)
                new_hour = (hour.split(':')[0])
            else:
                new_minute = str(60 - i)
                new_hour = str(int(hour.split(':')[0]) - 1) 
                
            new_hour_str = new_hour + ':' + new_minute
            new_price = round(random.uniform(low,high), 2)
            aux_dict = {'price': new_price,'date': date ,'hour': new_hour_str}
            data_mod = data_mod.append(aux_dict, ignore_index=True)
            
data_mod.drop(columns=['high','low','close'],inplace=True)

for date in data_mod['date'].unique():
    df_aux = data_mod[data_mod['date'] == date]
    df_aux = df_aux.reindex(np.random.permutation(df_aux.index))
    df_aux.to_csv(os.getcwd() + '/preprocessed_data/' + date + '.csv', index = False, header=False)