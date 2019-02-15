--SELECT RUEDUS,
--       RUEDBT,
--       RUEDTN,
--       RURMK,
--       RUEDLN,
--       RUCKNU,
--       RUKCO,
--       RUGMFD,
--       RUEUPS,
--       RUICUT,
--       RUDGJ,
--       RUDMTJ,
--       RUCKAM,
--       RUAG,
--       RUGLBA,
--       RUCBNK,
--       RUTNST
--FROM
--(
    SELECT CASE e.company
               WHEN '00001' THEN
                   '820'
           END RUEDUS,
           CASE
               WHEN ISNULL(h.USR5, '') = '' THEN
                   LEFT(ISNULL(d.CHKNUM, ''), 15)
               ELSE
                   LEFT(ISNULL(h.USR5, ''), 15)
           END RUEDBT,
           d.EDOC RUEDTN,
           LEFT(ISNULL(d.TP_Name, ''), 30) RURMK,
           ROW_NUMBER() OVER (PARTITION BY d.EDOC
                              ORDER BY d.EDOC,
                                       CASE
                                           WHEN CHARINDEX('*', ISNULL(d.USR2, 0)) = 0 THEN
                                               ISNULL(d.CMAMT, ISNULL(d.INVAMT, 0)) * 100
                                           ELSE
                                               ROUND(
                                                        CAST(LEFT(ISNULL(d.USR2, 0), CHARINDEX('*', ISNULL(d.USR2, 0))
                                                                                     - 1) AS FLOAT) * 100,
                                                        0
                                                    )
                                       END DESC
                             ) * 1000 AS RUEDLN,
           ISNULL(d.CHKNUM, '') RUCKNU,
           e.company RUKCO,
           CASE e.company
               WHEN '00001' THEN
                   ISNULL(CAST(d.INVNUM AS VARCHAR), ISNULL(d.CMNUM, ''))
           END RUGMFD,
           'B' RUEUPS,
           '9B' RUICUT,
           --ISNULL(
           --          (100 + YEAR(h.TRANS_DATE) - 2000) * 1000
           --          + DATEDIFF(d, CONVERT(DATETIME, ('01/01/' + CONVERT(CHAR(4), YEAR(h.TRANS_DATE)))), h.TRANS_DATE)
           --          + 1,
           --          0
           --      ) 
				 tr.JDEDATE AS RUDGJ, --tr.JDEDate,h.TRANS_DATE,
           --ISNULL(
           --          (100 + YEAR(h.TRANS_DATE) - 2000) * 1000
           --          + DATEDIFF(d, CONVERT(DATETIME, ('01/01/' + CONVERT(CHAR(4), YEAR(h.TRANS_DATE)))), h.TRANS_DATE)
           --          + 1,
           --          0
           --      ) 
				 tr.JDEDATE AS RUDMTJ,
           CASE ROW_NUMBER() OVER (PARTITION BY d.EDOC
                                   ORDER BY d.EDOC,
                                            CASE
                                                WHEN CHARINDEX('*', ISNULL(d.USR2, 0)) = 0 THEN
                                                    ISNULL(d.CMAMT, ISNULL(d.INVAMT, 0)) * 100
                                                ELSE
                                                    ROUND(
                                                             CAST(LEFT(ISNULL(d.USR2, 0), CHARINDEX(
                                                                                                       '*',
                                                                                                       ISNULL(d.USR2, 0)
                                                                                                   ) - 1) AS FLOAT)
                                                             * 100,
                                                             0
                                                         )
                                            END DESC
                                  )
               WHEN 1 THEN
                   ROUND(ISNULL(h.CHKAMT, 0) * 100, 0)
               ELSE
                   NULL
           END RUCKAM,
           --CASE
           --    WHEN CHARINDEX('*', ISNULL(d.USR2, 0)) = 0 THEN
           --        ISNULL(d.CMAMT, ISNULL(d.PAYAMT, 0)) * 100
           --    ELSE
           --        ROUND(CAST(LEFT(ISNULL(d.USR2, 0), CHARINDEX('*', ISNULL(d.USR2, 0)) - 1) AS FLOAT) * 100, 0)
           --END usr2,
           CASE
               WHEN e.new_math = 'y' THEN
                   CASE
                       WHEN CHARINDEX('*', ISNULL(d.USR2, 0)) = 0 THEN
                           ISNULL(d.CMAMT, ISNULL(d.INVAMT, 0) - ISNULL(d.DISCAMT, 0)) * 100
                       ELSE
                           ROUND(CAST(LEFT(ISNULL(d.USR2, 0), CHARINDEX('*', ISNULL(d.USR2, 0)) - 1) AS FLOAT) * 100, 0)
                   END
               ELSE
                   CASE
                       WHEN CHARINDEX('*', ISNULL(d.USR2, 0)) = 0 THEN
                           ISNULL(d.CMAMT, ISNULL(d.PAYAMT, 0)) * 100
                       ELSE
                           ROUND(CAST(LEFT(ISNULL(d.USR2, 0), CHARINDEX('*', ISNULL(d.USR2, 0)) - 1) AS FLOAT) * 100, 0)
                   END
           END AS RUAG,
           ISNULL(e.GLBA, '') RUGLBA,
           CASE
               WHEN CHARINDEX('*', ISNULL(d.TP_Name, '')) = 0 THEN
                   ''
               ELSE
                   RIGHT(d.TP_Name, CHARINDEX('*', REVERSE(ISNULL(d.TP_Name, ''))) - 1)
           END RUCBNK,
           CASE
               WHEN CHARINDEX('*', ISNULL(d.TP_Name, '')) = 0 THEN
                   ''
               ELSE
                   LEFT(d.TP_Name, CHARINDEX('*', ISNULL(d.TP_Name, '')) - 1)
           END RUTNST,
           ISNULL(d.PROC_FLAG, '') AS PROCFLAG
    FROM EDI_820_HEADER h
        INNER JOIN EDI_820_DETAIL d
            ON h.EDOC = d.EDOC
        LEFT OUTER JOIN edi_usr5 e
            ON ISNULL(h.TP_DUNS, '') = ISNULL(e.tp_duns, '')
		LEFT JOIN dbatools.dbo.DateDimension tr ON h.TRANS_DATE = tr.DateKey
    WHERE ISNULL(d.TP_DUNS, '') NOT IN
          (
              SELECT ISNULL(tp_duns, '') FROM edi_usr5 WHERE loc IN ( 'CB' )
          )
          AND CASE e.company
                  WHEN '00001' THEN
                      ISNULL(CAST(d.INVNUM AS VARCHAR), ISNULL(d.CMNUM, ''))
              END != ''
--) AS a;


--SELECT * FROM dbatools.dbo.DateDimension