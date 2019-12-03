#Excelのファイルであるxlsxファイルを読み込むために、xlrdライブラリを読み込む
import xlrd
#なんかコンソールに出力させるための
import pprint


#ファイルをオープンする、withでcloseをしなくていいらしい
with open('../../GameTemplate/Game/Assets/binarydata/test.cag','w+b') as file:
    #Excelのファイルを読み込む
    book = xlrd.open_workbook('file/Book1.xlsx')
    #エクセルファイルの一枚目のシートを取得
    sheet = book.sheet_by_index(0) 
    #シートの行の数だけ繰り返す
    for row_index in range(sheet.nrows - 1):
        #シートの行をリストとして取得
        col_values = sheet.row_values(row_index + 1)
        #シートの列の数だけ繰り返す
        for i in range(len(sheet.row_values(row_index + 1))):
            if i == 0:
                 dat =  (str(col_values[i]) + " ").encode()
                 print(type(dat),dat)
                 file.write(dat)
            elif i == 1:
                 dat =  (str(col_values[i]) + " ").encode()
                 print(type(dat),dat)
                 file.write(dat)
            else:
                #列の最後の要素を書き込むとき、改行する
                if i + 1 == len(sheet.row_values(row_index + 1)):
                 dat =  (str(int(col_values[i])) + "\n").encode()
                 print(type(dat),dat)
                 file.write(dat)
                else:
                 dat =  (str(int(col_values[i])) + " ").encode()
                 print(type(dat),dat)
                 file.write(dat)


 
