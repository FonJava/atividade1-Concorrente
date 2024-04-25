**Atividade: Sistema de Reserva de Ingressos**

Descrição: Desenvolva um sistema de reserva de ingressos concorrente que permita que múltiplos compradores reservem ingressos para um evento. O sistema deve garantir que o número total de ingressos reservados não exceda a capacidade máxima do evento.

*Arquitetura do Sistema:*

O sistema é composto por múltiplas threads compradoras (`compra_ingresso`) e threads produtoras (`produtor_de_ingressos`), além das estruturas de dados compartilhadas e dos mecanismos de sincronização.

*Threads:*

- Threads Compradoras (`compra_ingresso`):
     * Geram um número aleatório de ingressos a serem comprados.
     * Verificam se há ingressos disponíveis para a compra.
     * Reservam os ingressos necessários, adicionando a compra à fila de compras compartilhada.
     * Aguardam se não houver ingressos disponíveis.
     * São sincronizadas com as threads produtoras por meio de variáveis de condição.

- Threads Produtoras (`produtor_de_ingressos`):
   * Verificam continuamente a fila de compras para detectar novas compras.
   * As threads produtoras repõem a mesma quantidade de ingressos que foram reservados na última compra.
   * São sincronizadas com as threads compradoras por meio de variáveis de condição.

*Sincronização:*

- **Mutex (`mutex`):** Um mutex é utilizado para garantir acesso seguro às estruturas de dados compartilhadas, como a fila de compras e a variável de ingressos disponíveis. Ele evita condições de corrida e garante que apenas uma thread por vez execute a seção crítica do código.

- **Variáveis de Condição (`cond_not_full` e `cond_not_empty`):** Duas variáveis de condição são usadas para sincronizar as operações entre as threads compradoras e produtoras. 
   - `cond_not_full` é usada pelas threads produtoras para esperar se não houver compras na fila de compras.
   - `cond_not_empty` é usada pelas threads compradoras para esperar se não houver ingressos suficientes para compra.

*Mecanismo de Feedback de Reserva:*

- Após cada transação de reserva, o sistema fornece feedback usando o printf para indicar o número de ingressos reservados e os ingressos disponíveis após a reserva.
- Se não houver ingressos suficientes disponíveis, as threads compradoras aguardam até que mais ingressos estejam disponíveis, enquanto as threads produtoras adicionam ingressos conforme necessário.
- O sistema garante que o número total de ingressos reservados não exceda a capacidade máxima do evento, mantendo o controle sobre a disponibilidade de ingressos e o número de compras na fila.
