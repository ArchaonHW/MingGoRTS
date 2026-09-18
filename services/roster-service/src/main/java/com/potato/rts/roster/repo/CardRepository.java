package com.potato.rts.roster.repo;

import com.potato.rts.roster.model.Card;
import org.springframework.data.jpa.repository.JpaRepository;

public interface CardRepository extends JpaRepository<Card, String> {
}
