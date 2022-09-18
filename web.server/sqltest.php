<!DOCTYPE html>
<html>
    <head>
        <title>PHP MySQL Stored Procedure Demo 1</title>
        <link rel="stylesheet" href="css/table.css" type="text/css" />
    </head>
    <body>
        <?php
        require_once '_config.php';
        try {
            $pdo = new PDO("mysql:host=$host;dbname=$dbname", $user, $password);
            // execute the stored procedure
            $sql = 'CALL get_active_messages()';
            // call the stored procedure
            $q = $pdo->query($sql);
            $q->setFetchMode(PDO::FETCH_ASSOC);
        } catch (PDOException $e) {
            die("Error occurred:" . $e->getMessage());
        }
        ?>
        <table>
            <tr>
                <th>Customer Name</th>
                <th>Credit Limit</th>
            </tr>
            <?php while ($r = $q->fetch()): ?>
                <tr>
                    <td><?php echo $r['source'] ?></td>
                    <td><?php echo '$' . number_format($r['id'], 2) ?>
                    </td>
                </tr>
            <?php endwhile; ?>
        </table>
    </body>
</html>

